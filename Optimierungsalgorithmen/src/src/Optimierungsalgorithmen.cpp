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
#include "stdafx.h"


Optimierungsalgorithmen::Optimierungsalgorithmen(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    mainScene_ = new MainScene(this);


    mainWindow_ = new MainWindow(this, mainScene_);
    this->setCentralWidget(mainWindow_);

    algoSelectionUI_ = new AlgorithmSelectionUI();
    dataHolder_ = std::make_shared<DataHolder>();

    
    initSol_ = new SimpleInitialSolution<DataHolder*>();
    ruleBasedNeighbour_ = new RuleBasedNeighbour<DataHolder*>(dataHolder_.get(), initSol_);
    //neighbourWrapper_ = std::make_shared<QNeighbourWrapper>(ruleBasedNeighbour_);
    neighbourWrapper_ = new QNeighbourWrapper(ruleBasedNeighbour_);


    connect(algoSelectionUI_->getRecAmountSlider(), &QSlider::valueChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectAmountChangedI);
    connect(algoSelectionUI_->getRecMaxSizeSlider(), &QSlider::valueChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectSizeChangedI);
    connect(algoSelectionUI_->getRecAmountLineEdit(), &QLineEdit::textChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectAmountChangedS);
    connect(algoSelectionUI_->getRecMaxSizeLineEdit(), &QLineEdit::textChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectSizeChangedS);

    connect(algoSelectionUI_->getBoxEdgeSlider(), &QSlider::valueChanged, mainScene_->getBoxDrawer(), &BoundingBoxDrawer::BoundingBoxSizeChangedI);
    connect(algoSelectionUI_->getBoxEdgeLineEdit(), & QLineEdit::textChanged, mainScene_->getBoxDrawer(), &BoundingBoxDrawer::BoundingBoxSizeChangedS);
    connect(mainScene_->getBoxDrawer(), &BoundingBoxDrawer::BoundingBoxSizeChanged, dataHolder_->getBoxCreator().get(), &BoundingBoxCreator::EdgeLengthChanged);
    connect(mainScene_->getBoxDrawer(), &BoundingBoxDrawer::BoundingBoxSizeChanged, mainScene_->getRecDrawer(), &RectangleDrawer::OnBoundingBoxSizeChanged);

    connect(ruleBasedNeighbour_, &SignalHelper::OptimDone, dataHolder_->getBoxCreator().get(), &BoundingBoxCreator::OnOptimDone);
    connect(dataHolder_->getBoxCreator().get(), &BoundingBoxCreator::EmitRectList, mainScene_->getBoxDrawer(), &BoundingBoxDrawer::SetBoundingBoxes);

    connect(dataHolder_->getRectCreator().get(), &RectangleCreator::RectListCreated, neighbourWrapper_, &QNeighbourWrapper::Optim);

    connect(mainScene_->getRecDrawer(), &RectangleDrawer::EmitRectInformation, dataHolder_->getRectCreator().get(), &RectangleCreator::CreateRects);

    connect(dataHolder_->getRectCreator().get(), &RectangleCreator::RectListUpdated, mainScene_->getRecDrawer(), &RectangleDrawer::SetRects);
  
    leftDock_ = new QDockWidget(this);
    leftDock_->setTitleBarWidget(new QWidget());
    leftDock_->setFeatures(QDockWidget::NoDockWidgetFeatures);
    leftDock_->setFixedSize(UIConstants::leftDockWidth_, UIConstants::leftDockHeight_);
    leftDock_->setWidget(algoSelectionUI_);
    this->addDockWidget(Qt::LeftDockWidgetArea, leftDock_);
    
   


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

    delete ruleBasedNeighbour_;
    ruleBasedNeighbour_ = nullptr;

    delete neighbourWrapper_;
    neighbourWrapper_ = nullptr;

    delete initSol_;
    initSol_ = nullptr;


}
