#include "MainScene.h"
#include "RectangleDrawer.h"
MainScene::MainScene(QMainWindow* win) : QGraphicsScene(win) {
	
	//connect(this, &MainScene::Clicked, this, &MainScene::DrawRect);
	
	recDrawer_ = new RectangleDrawer(this);

}
MainScene::~MainScene()
{
	delete recDrawer_;
	recDrawer_ = nullptr;
}

RectangleDrawer* MainScene::getRecDrawer() const
{
	return recDrawer_;
}

void MainScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mouseEvent) 
{
	emit Clicked(mouseEvent->scenePos());
	mouseEvent->accept();
}

void MainScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
	std::cout << mouseEvent->scenePos().x() << " " << mouseEvent->scenePos().y() << std::endl;
	mouseEvent->accept();
}

void MainScene::DrawRect(QPointF p) {
	//std::cout << p.x() << " " << p.y() << std::endl;
	//QRectF r(p.x(), p.y(), 200, 100);
	this->addRect(p.x(), p.y(), 200, 100);
	this->setFocus();
	//r.moveTo(p.x(), p.y());
}