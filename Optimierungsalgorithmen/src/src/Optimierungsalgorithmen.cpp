#include "Optimierungsalgorithmen.h"
#include "MainWindow.h"
#include "AlgorithmSelectionUI.h"
#include "MainScene.h"
#include "RectangleDrawer.h"
#include "stdafx.h"


Optimierungsalgorithmen::Optimierungsalgorithmen(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    mainScene_ = new MainScene(this);


    mainWindow_ = new MainWindow(this, mainScene_);
    this->setCentralWidget(mainWindow_);

    algoSelectionUI_ = new AlgorithmSelectionUI();

    connect(algoSelectionUI_->getRecAmountSlider(), &QSlider::valueChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectAmountChangedI);
    connect(algoSelectionUI_->getRecMaxSizeSlider(), &QSlider::valueChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectSizeChangedI);
    connect(algoSelectionUI_->getRecAmountLineEdit(), &QLineEdit::textChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectAmountChangedS);
    connect(algoSelectionUI_->getRecMaxSizeLineEdit(), &QLineEdit::textChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectSizeChangedS);
  
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


}
