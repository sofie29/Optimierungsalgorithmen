#include "Drawer.h"
#include "BoundingBoxDrawer.h"
#include "RectangleDrawer.h"
Drawer::Drawer(QGraphicsScene* scene) : scene_(scene) {
	boundingBoxDrawer_ = new BoundingBoxDrawer();
	rectDrawer_ = new RectangleDrawer();

	//connect(boundingBoxDrawer_, &BoundingBoxDrawer::EmitListChanged, this, &Drawer::DrawScene);
	//connect(rectDrawer_, &RectangleDrawer::EmitListChanged, this, &Drawer::DrawScene);
}

Drawer::~Drawer()
{
	delete rectDrawer_;
	rectDrawer_ = nullptr;

	delete boundingBoxDrawer_;
	boundingBoxDrawer_ = nullptr;

	scene_ = nullptr;
}

RectangleDrawer* Drawer::getRectDrawer()
{
	return rectDrawer_;
}

BoundingBoxDrawer* Drawer::getBBoxDrawer()
{
	return boundingBoxDrawer_;
}

void Drawer::DrawScene() {
	scene_->clear();
	boundingBoxDrawer_->DrawOnScene(scene_);
	rectDrawer_->DrawOnScreen(scene_);
	scene_->update();
}
