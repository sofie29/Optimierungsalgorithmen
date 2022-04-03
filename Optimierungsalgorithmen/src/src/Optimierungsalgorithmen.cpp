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
Optimierungsalgorithmen::Optimierungsalgorithmen(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    mainScene_ = new MainScene(this);


    mainWindow_ = new MainWindow(this, mainScene_);
    this->setCentralWidget(mainWindow_);

    algoSelectionUI_ = new AlgorithmSelectionUI();

    dataHolder_ = new DataHolder();
    bestDataHolder_ = new DataHolder();
    dataHolderT_ = new DataHolderT<DataHolder*>(dataHolder_);
    bestDataHolderT_ = new DataHolderT<DataHolder*>(bestDataHolder_);


    initSol_ = new SimpleInitialSolution<DataHolder*>();
    emptyBoxObjective_ = new EmptyBoxObjective<DataHolder*>();
    simpleEmptyBoxObjective_ = new SimpleEmptyBoxObjective<DataHolder*>();
    boxListLengthObjective_ = new BoxListLengthObjective<DataHolder*>();

    ruleBasedNeighbour_ = new RuleBasedNeighbour<DataHolder*>(dataHolderT_, bestDataHolderT_, initSol_);
    geometryBasedNeighbour_ = new GeometryBasedNeighbour<DataHolder*>(dataHolderT_, bestDataHolderT_, initSol_);
    geometryBasedOverlappingNeighbour_ = new GeometryBasedOverlappingNeighbour<DataHolder*>(dataHolderT_, bestDataHolderT_, initSol_);
    emptyBoxObjective_->setNeighbour(geometryBasedOverlappingNeighbour_);

    neighbourWrapper_ = new QNeighbourWrapper(ruleBasedNeighbour_);
    localSearch_ = new LocalSearch<DataHolder*>(neighbourWrapper_->getNeighbourI(), dataHolderT_, bestDataHolderT_, initSol_, simpleEmptyBoxObjective_, boxListLengthObjective_);

    areaSortStrategy_ = new AreaSortingStrategy<DataHolder*>();
    diagonalSortStrategy_ = new DiagonalSortingStrategy<DataHolder*>();
    greedy_ = new Greedy<DataHolder*>(dataHolderT_, bestDataHolderT_, initSol_, areaSortStrategy_, simpleEmptyBoxObjective_, boxListLengthObjective_);

   

    selectedAlgorithm_ = localSearch_;

    algoWrapper_ = new QAlgoWrapper(selectedAlgorithm_);

    connect(algoSelectionUI_->getRecAmountSlider(), &QSlider::valueChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectAmountChangedI);
    connect(algoSelectionUI_->getRecAmountLineEdit(), &QLineEdit::textChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectAmountChangedS);

    connect(algoSelectionUI_->getRecMinWidthSlider(), &QSlider::valueChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectMinWidthChangedI);
    connect(algoSelectionUI_->getRecMinWidthLineEdit(), &QLineEdit::textChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectMinWidthChangedS);

    connect(algoSelectionUI_, &AlgorithmSelectionUI::EmitMaxWidthChangedI, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectMaxWidthChangedI);
    connect(algoSelectionUI_, &AlgorithmSelectionUI::EmitMaxWidthChangedS, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectMaxWidthChangedS);

    connect(algoSelectionUI_->getRecMinHeightSlider(), &QSlider::valueChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectMinHeightChangedI);
    connect(algoSelectionUI_->getRecMinHeightLineEdit(), &QLineEdit::textChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectMinHeightChangedS);

    connect(algoSelectionUI_, &AlgorithmSelectionUI::EmitMaxHeightChangedI, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectMaxHeightChangedI);
    connect(algoSelectionUI_, &AlgorithmSelectionUI::EmitMaxHeightChangedS, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectMaxHeightChangedS);

    connect(algoSelectionUI_->getBoxEdgeSlider(), &QSlider::valueChanged, mainScene_->getBoxDrawer(), &BoundingBoxDrawer::BoundingBoxSizeChangedI);
    connect(algoSelectionUI_->getBoxEdgeLineEdit(), &QLineEdit::textChanged, mainScene_->getBoxDrawer(), &BoundingBoxDrawer::BoundingBoxSizeChangedS);

    connect(mainScene_->getBoxDrawer(), &BoundingBoxDrawer::BoundingBoxSizeChanged, dataHolder_->getBoxCreator().get(), &BoundingBoxCreator::EdgeLengthChanged);
    connect(mainScene_->getBoxDrawer(), &BoundingBoxDrawer::BoundingBoxSizeChanged, bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::EdgeLengthChanged);

    //This sets boundingboxes after optimization is done
    connect(bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::EmitRectList, mainScene_->getBoxDrawer(), &BoundingBoxDrawer::SetBoundingBoxes);
    connect(bestDataHolder_->getRectCreator().get(), &RectangleCreator::EmitRectList, mainScene_->getRecDrawer(), &RectangleDrawer::SetRects);

    connect(localSearch_, &OptimAlgoI<DataHolder*>::OptimDone, bestDataHolder_->getRectCreator().get(), &RectangleCreator::OnOptimDone);
    connect(localSearch_, &OptimAlgoI<DataHolder*>::StepDone, bestDataHolder_->getRectCreator().get(), &RectangleCreator::OnOptimDone);
    connect(localSearch_, &OptimAlgoI<DataHolder*>::OptimDone, bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::OnOptimDone);
    connect(localSearch_, &OptimAlgoI<DataHolder*>::StepDone, bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::OnStepDone);
    connect(greedy_, &OptimAlgoI<DataHolder*>::OptimDone, bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::OnOptimDone);
    connect(greedy_, &OptimAlgoI<DataHolder*>::StepDone, bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::OnStepDone);
    connect(greedy_, &OptimAlgoI<DataHolder*>::OptimDone, bestDataHolder_->getRectCreator().get(), &RectangleCreator::OnOptimDone);
    connect(greedy_, &OptimAlgoI<DataHolder*>::StepDone, bestDataHolder_->getRectCreator().get(), &RectangleCreator::OnOptimDone);


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
    connect(localSearch_, &OptimAlgoI<DataHolder*>::DrawSolution, mainScene_->getDrawer(), &Drawer::DrawScene);
    connect(greedy_, &OptimAlgoI<DataHolder*>::DrawSolution, mainScene_->getDrawer(), &Drawer::DrawScene);

    connect(localSearch_, &OptimAlgoI<DataHolder*>::DrawSwappedRects, mainScene_->getDrawer(), &Drawer::DrawSwappedRects);

    //Step Logic
    connect(localSearch_, &OptimAlgoI<DataHolder*>::EmitCurrentStep, algoSelectionUI_, &AlgorithmSelectionUI::setCurrentStepNumberLabel);
    connect(greedy_, &OptimAlgoI<DataHolder*>::EmitCurrentStep, algoSelectionUI_, &AlgorithmSelectionUI::setCurrentStepNumberLabel);
    connect(algoSelectionUI_->getNextStepButton(), &QPushButton::clicked, algoWrapper_, &QAlgoWrapper::RunOneStep);

    //Reset Button
    connect(algoSelectionUI_->getResetButton(), &QPushButton::clicked, algoWrapper_, &QAlgoWrapper::Reset);

    //Timer Logic
    connect(localSearch_, &OptimAlgoI<DataHolder*>::EmitTakenTime, algoSelectionUI_, &AlgorithmSelectionUI::setTimeDisplayLabel);
    connect(localSearch_, &OptimAlgoI<DataHolder*>::EmitTakenTimeAvg, algoSelectionUI_, &AlgorithmSelectionUI::setAvgTimeDisplayLabel);

    connect(greedy_, &OptimAlgoI<DataHolder*>::EmitTakenTime, algoSelectionUI_, &AlgorithmSelectionUI::setTimeDisplayLabel);
    connect(greedy_, &OptimAlgoI<DataHolder*>::EmitTakenTimeAvg, algoSelectionUI_, &AlgorithmSelectionUI::setAvgTimeDisplayLabel);

    leftDock_ = new QDockWidget(this);
    leftDock_->setTitleBarWidget(new QWidget());
    leftDock_->setFeatures(QDockWidget::NoDockWidgetFeatures);
    leftDock_->setFixedSize(UIConstants::leftDockWidth_, UIConstants::leftDockHeight_);
    leftDock_->setWidget(algoSelectionUI_);
    this->addDockWidget(Qt::LeftDockWidgetArea, leftDock_);

    dataHolder_->getRectCreator()->CreateRects(AlgorithmConstants::initialAmount_, AlgorithmConstants::initialEdgeSize_, AlgorithmConstants::initialEdgeSize_, AlgorithmConstants::initialEdgeSize_, AlgorithmConstants::initialEdgeSize_);



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

    delete areaSortStrategy_;
    areaSortStrategy_ = nullptr;

    delete diagonalSortStrategy_;
    diagonalSortStrategy_ = nullptr;

    delete greedy_;
    greedy_ = nullptr;

    delete emptyBoxObjective_;
    emptyBoxObjective_ = nullptr;

    delete simpleEmptyBoxObjective_;
    simpleEmptyBoxObjective_ = nullptr;

    delete boxListLengthObjective_;
    boxListLengthObjective_ = nullptr;

}

void Optimierungsalgorithmen::changeAlgorithm(int idx)
{
    switch (idx) {
    case 0:
        neighbourWrapper_->setNeighbour(ruleBasedNeighbour_);
        localSearch_->setNeighbourDefinition(ruleBasedNeighbour_);
        emptyBoxObjective_->setNeighbour(ruleBasedNeighbour_);
        localSearch_->setObjective(simpleEmptyBoxObjective_);
        selectedAlgorithm_ = localSearch_;
        algoWrapper_->setMod(2);
        break;
    case 1:
        neighbourWrapper_->setNeighbour(geometryBasedNeighbour_);
        localSearch_->setNeighbourDefinition(geometryBasedNeighbour_);
        emptyBoxObjective_->setNeighbour(geometryBasedNeighbour_);
        localSearch_->setObjective(simpleEmptyBoxObjective_);
        selectedAlgorithm_ = localSearch_;
        algoWrapper_->setMod(2);
        break;
    case 2:
        neighbourWrapper_->setNeighbour(geometryBasedOverlappingNeighbour_);
        localSearch_->setNeighbourDefinition(geometryBasedOverlappingNeighbour_);
        emptyBoxObjective_->setNeighbour(geometryBasedOverlappingNeighbour_);
        localSearch_->setObjective(emptyBoxObjective_);
        selectedAlgorithm_ = localSearch_;
        algoWrapper_->setMod(2);
        break;
    case 3:
        greedy_->setSortStrat(areaSortStrategy_);
        greedy_->setObjective(emptyBoxObjective_);
        selectedAlgorithm_ = greedy_;
        algoWrapper_->setMod(1);
        break;
    case 4:
        greedy_->setSortStrat(diagonalSortStrategy_);
        greedy_->setObjective(emptyBoxObjective_);
        selectedAlgorithm_ = greedy_;
        algoWrapper_->setMod(1);
        break;
    }
    algoWrapper_->setAlgorithm(selectedAlgorithm_);
    algoWrapper_->Reset();
}

