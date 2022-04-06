#include "Drawer.h"
#include "BoundingBoxDrawer.h"
#include "RectangleDrawer.h"
#include "BoundingBox.h"
#include "BoundingBoxCreator.h"
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

void Drawer::DrawSwappedRects()
{
	scene_->clear();
	boundingBoxDrawer_->DrawOnScene(scene_, true);
	rectDrawer_->DrawOnScreen(scene_, true);
	scene_->update();
}

void Drawer::rearrangeBoxes(BoundingBoxCreator* bBoxCreator)
{
	std::vector<QRectF> bBoxRects;
	boundingBoxDrawer_->getBoundingBoxList(bBoxRects);

	std::vector<std::shared_ptr<BoundingBox>> bBoxBoxes;
	bBoxCreator->getBoundingBoxList(bBoxBoxes);

	std::vector<QRectF> rectRects;
	rectDrawer_->getRects(rectRects);

	int idx = 0;
	int boxesPerLine = 13;
	for (QRectF& bBox : bBoxRects) {
		int x_pos = (idx % boxesPerLine) * (AlgorithmConstants::maxBoxEdgeSize_ + UIConstants::rectangleSpace_);
		float val = std::floor((float)idx / (float)boxesPerLine);
		int y_pos = (int)val * (AlgorithmConstants::maxBoxEdgeSize_ + UIConstants::rectangleSpace_);
	
		int x_diff = x_pos - bBox.x();
		int y_diff = y_pos - bBox.y();
		bBox.moveTopLeft(QPoint(x_pos, y_pos));
		std::vector<int> rectIdx = bBoxBoxes[idx]->getRectangleIndices();
		for (int rectIndex : rectIdx) {
			int rectNewX = rectRects[rectIndex].x() + x_diff;
			int rectNewY = rectRects[rectIndex].y() + y_diff;
			rectRects[rectIndex].moveTopLeft(QPoint(rectNewX, rectNewY));
		}


		++idx;
	}
	rectDrawer_->setRectsQRectF(rectRects);
	boundingBoxDrawer_->SetBoundingBoxes(bBoxRects);
}

void Drawer::DrawScene(class BoundingBoxCreator* bBoxCreator) {
	rearrangeBoxes(bBoxCreator);
	scene_->clear();
	boundingBoxDrawer_->DrawOnScene(scene_, false);
	rectDrawer_->DrawOnScreen(scene_, false);
	scene_->update();
}
