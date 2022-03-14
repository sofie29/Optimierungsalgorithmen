#include "BoundingBoxDrawer.h"

BoundingBoxDrawer::BoundingBoxDrawer()  {

	currentBoxLength_ = AlgorithmConstants::initialBoxEdgeSize_;
}
BoundingBoxDrawer::~BoundingBoxDrawer() {
	
}
void BoundingBoxDrawer::DrawOnScene(QGraphicsScene* scene)
{
	for (const QRectF& rect : boundingBoxList_) {
		scene->addRect(rect);
	}
}
void BoundingBoxDrawer::BoundingBoxSizeChangedS(const QString& maxEdgeLength)
{
	if (maxEdgeLength.toInt() <= AlgorithmConstants::maxBoxEdgeSize_) {
		currentBoxLength_ = maxEdgeLength.toInt();
		emit BoundingBoxSizeChanged(currentBoxLength_);
	}
}

void BoundingBoxDrawer::BoundingBoxSizeChangedI(const int maxEdgeLength)
{
	if (maxEdgeLength <= AlgorithmConstants::maxBoxEdgeSize_) {
		currentBoxLength_ = maxEdgeLength;
		emit BoundingBoxSizeChanged(currentBoxLength_);
	}
}

void BoundingBoxDrawer::SetBoundingBoxes(const std::vector<QRectF>& list)
{
	boundingBoxList_ = list;
	emit EmitListChanged();
	/*
	scene_->clear();
	for (QRectF rect : list) {
		scene_->addRect(rect);
	}
	scene_->update();
	*/
}
