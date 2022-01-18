#include "MainWindow.h"

MainWindow::MainWindow(QMainWindow* win, QGraphicsScene* scene) : QGraphicsView(win)
{
	this->setInteractive(false);
	this->setScene(scene);
	this->setFixedSize(UIConstants::mainWindowWidth_, UIConstants::mainWindowHeight_);
	QRectF r(0, 0, UIConstants::mainWSceneWidth_, UIConstants::mainSceneHeight_);
	this->setSceneRect(r);
}

MainWindow::~MainWindow()
{
}

