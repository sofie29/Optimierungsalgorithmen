#include "Optimierungsalgorithmen.h"
#include "MainWindow.h"
#include "AlgorithmSelectionUI.h"
#include "MainScene.h"
#include "RectangleDrawer.h"
#include "BoundingBoxDrawer.h"
#include "DataHolder.h"
#include "RectangleCreator.h"
#include "QNeighbourWrapper.h"
#include "InitialSolutionI.h"
#include "SimpleInitialSolution.h"
#include "OptimAlgoI.h"
#include "QAlgoWrapper.h"
#include "Drawer.h"
#include "stdafx.h"

//#include "Memory.h"
Optimierungsalgorithmen::Optimierungsalgorithmen(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    mainScene_ = new MainScene(this);


    mainWindow_ = new MainWindow(this, mainScene_);
    this->setCentralWidget(mainWindow_);

    algoSelectionUI_ = new AlgorithmSelectionUI();

    dataHolder_ =  new DataHolder();
    bestDataHolder_ = new DataHolder();
    dataHolderT_ = new DataHolderT<DataHolder*>(dataHolder_);
    bestDataHolderT_ = new DataHolderT<DataHolder*>(bestDataHolder_);
   

    initSol_ = new SimpleInitialSolution<DataHolder*>();
    ruleBasedNeighbour_ = new RuleBasedNeighbour<DataHolder*>(dataHolderT_, bestDataHolderT_, initSol_);
    geometryBasedNeighbour_ = new GeometryBasedNeighbour<DataHolder*>(dataHolderT_, bestDataHolderT_, initSol_);
    geometryBasedOverlappingNeighbour_ = new GeometryBasedOverlappingNeighbour<DataHolder*>(dataHolderT_, bestDataHolderT_, initSol_);
    neighbourWrapper_ = new QNeighbourWrapper(ruleBasedNeighbour_);
    localSearch_ = new LocalSearch<DataHolder*>(neighbourWrapper_->getNeighbourI(), dataHolderT_, bestDataHolderT_, initSol_);
    selectedAlgorithm_ = localSearch_;

    algoWrapper_ = new QAlgoWrapper(selectedAlgorithm_);

    connect(algoSelectionUI_->getRecAmountSlider(), &QSlider::valueChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectAmountChangedI);
    connect(algoSelectionUI_->getRecMaxSizeSlider(), &QSlider::valueChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectSizeChangedI);
    connect(algoSelectionUI_->getRecAmountLineEdit(), &QLineEdit::textChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectAmountChangedS);
    connect(algoSelectionUI_->getRecMaxSizeLineEdit(), &QLineEdit::textChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectSizeChangedS);
    connect(algoSelectionUI_->getBoxEdgeSlider(), &QSlider::valueChanged, mainScene_->getBoxDrawer(), &BoundingBoxDrawer::BoundingBoxSizeChangedI);
    connect(algoSelectionUI_->getBoxEdgeLineEdit(), & QLineEdit::textChanged, mainScene_->getBoxDrawer(), &BoundingBoxDrawer::BoundingBoxSizeChangedS);

    connect(mainScene_->getBoxDrawer(), &BoundingBoxDrawer::BoundingBoxSizeChanged, dataHolder_->getBoxCreator().get(), &BoundingBoxCreator::EdgeLengthChanged);
    connect(mainScene_->getBoxDrawer(), &BoundingBoxDrawer::BoundingBoxSizeChanged, bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::EdgeLengthChanged);

    //This sets boundingboxes after optimization is done
    connect(selectedAlgorithm_, &OptimAlgoI<DataHolder*>::OptimDone, bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::OnOptimDone);
    connect(selectedAlgorithm_, &OptimAlgoI<DataHolder*>::StepDone, bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::OnStepDone);
    connect(bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::EmitRectList, mainScene_->getBoxDrawer(), &BoundingBoxDrawer::SetBoundingBoxes);
    connect(selectedAlgorithm_, &OptimAlgoI<DataHolder*>::OptimDone, bestDataHolder_->getRectCreator().get(), &RectangleCreator::OnOptimDone);
    connect(selectedAlgorithm_, &OptimAlgoI<DataHolder*>::StepDone, bestDataHolder_->getRectCreator().get(), &RectangleCreator::OnOptimDone);
    connect(bestDataHolder_->getRectCreator().get(), &RectangleCreator::EmitRectList, mainScene_->getRecDrawer(), &RectangleDrawer::SetRects);

    //Set FitScore after optim is done
    connect(dataHolder_->getRectCreator().get(), &RectangleCreator::EmitSquareSize, bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::RecSquareSizeChanged);
    connect(dataHolder_->getRectCreator().get(), &RectangleCreator::EmitSquareSize, dataHolder_->getBoxCreator().get(), &BoundingBoxCreator::RecSquareSizeChanged);
    connect(bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::EmitNumberOfBoundingBoxes, algoSelectionUI_, &AlgorithmSelectionUI::setCurrentNumberLabel);
    connect(bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::EmitOptimalAmountRect, algoSelectionUI_, &AlgorithmSelectionUI::setOptimNumberLabel);
    connect(bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::EmitCurrentFitScore, algoSelectionUI_, &AlgorithmSelectionUI::setCurrentOptimPctLabel);

    //Upon changing parameters this updates rectList
    connect(mainScene_->getRecDrawer(), &RectangleDrawer::EmitRectInformation, dataHolder_->getRectCreator().get(), &RectangleCreator::CreateRects);
    connect(dataHolder_->getRectCreator().get(), &RectangleCreator::RectListUpdated, mainScene_->getRecDrawer(), &RectangleDrawer::SetRects);

    //AlgorithmChange/Start Logic
    connect(algoSelectionUI_->getAlgoSelectionBox(), &QComboBox::currentIndexChanged, this, &Optimierungsalgorithmen::changeAlgorithm);
    connect(algoSelectionUI_->getStartButton(), &QPushButton::clicked, algoWrapper_, &QAlgoWrapper::RunUntilTermination);

    //Draw Solution
    connect(selectedAlgorithm_, &OptimAlgoI<DataHolder*>::DrawSolution, mainScene_->getDrawer(), &Drawer::DrawScene);

    //Step Logic
    connect(selectedAlgorithm_, &OptimAlgoI<DataHolder*>::EmitCurrentStep, algoSelectionUI_, &AlgorithmSelectionUI::setCurrentStepNumberLabel);
    connect(algoSelectionUI_->getNextStepButton(), &QPushButton::clicked, algoWrapper_, &QAlgoWrapper::RunOneStep);
    
    //Reset Button
    connect(algoSelectionUI_->getResetButton(), &QPushButton::clicked, algoWrapper_, &QAlgoWrapper::Reset);

    //Timer Logic
    connect(selectedAlgorithm_, &OptimAlgoI<DataHolder*>::EmitTakenTime, algoSelectionUI_, &AlgorithmSelectionUI::setTimeDisplayLabel);
    connect(selectedAlgorithm_, &OptimAlgoI<DataHolder*>::EmitTakenTimeAvg, algoSelectionUI_, &AlgorithmSelectionUI::setAvgTimeDisplayLabel);

    leftDock_ = new QDockWidget(this);
    leftDock_->setTitleBarWidget(new QWidget());
    leftDock_->setFeatures(QDockWidget::NoDockWidgetFeatures);
    leftDock_->setFixedSize(UIConstants::leftDockWidth_, UIConstants::leftDockHeight_);
    leftDock_->setWidget(algoSelectionUI_);
    this->addDockWidget(Qt::LeftDockWidgetArea, leftDock_);
    
    dataHolder_->getRectCreator()->CreateRects(AlgorithmConstants::initialAmount_, AlgorithmConstants::initialEdgeSize_);
   


}

Optimierungsalgorithmen::~Optimierungsalgorithmen()
{
    
    delete algoSelectionUI_;
    algoSelectionUI_ = nullptr;

    delete leftDock_;
    leftDock_ = nullptr;

    delete mainWindow_;
    mainWindow_ = nullptr;

    delete mainScene_;
    mainScene_ = nullptr;

    delete geometryBasedNeighbour_;
    geometryBasedNeighbour_ = nullptr;
    
    delete geometryBasedOverlappingNeighbour_;
    geometryBasedOverlappingNeighbour_ = nullptr;

    delete algoWrapper_;
    algoWrapper_ = nullptr;

    delete ruleBasedNeighbour_;
    ruleBasedNeighbour_ = nullptr;

    delete neighbourWrapper_;
    neighbourWrapper_ = nullptr;

    delete initSol_;
    initSol_ = nullptr;

    delete localSearch_;
    localSearch_ = nullptr;

    delete dataHolderT_;
    dataHolderT_ = nullptr;

    delete bestDataHolderT_;
    bestDataHolderT_ = nullptr;

    delete dataHolder_;
    dataHolder_ = nullptr;

    delete bestDataHolder_;
    bestDataHolder_ = nullptr;

}

void Optimierungsalgorithmen::changeAlgorithm(int idx)
{
    switch (idx) {
    case 0:
        neighbourWrapper_->setNeighbour(ruleBasedNeighbour_);
        localSearch_->setNeighbourDefinition(ruleBasedNeighbour_);
        selectedAlgorithm_ = localSearch_;
        break;
    case 1:
        neighbourWrapper_->setNeighbour(geometryBasedNeighbour_);
        localSearch_->setNeighbourDefinition(geometryBasedNeighbour_);
        selectedAlgorithm_ = localSearch_;
        break;
    case 2:
        neighbourWrapper_->setNeighbour(geometryBasedOverlappingNeighbour_);
        localSearch_->setNeighbourDefinition(geometryBasedOverlappingNeighbour_);
        selectedAlgorithm_ = localSearch_;
        break;
    case 3:
        selectedAlgorithm_ = nullptr;
        break;
    }
    algoWrapper_->setAlgorithm(selectedAlgorithm_);
    algoWrapper_->Reset();
}

