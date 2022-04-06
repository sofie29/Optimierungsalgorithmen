#include "BoundingBoxDrawer.h"

BoundingBoxDrawer::BoundingBoxDrawer()  {

	currentBoxLength_ = AlgorithmConstants::initialBoxEdgeSize_;
}
BoundingBoxDrawer::~BoundingBoxDrawer() {
	boundingBoxList_.clear();
	boundingBoxList_.shrink_to_fit();

	boundingBoxListOld_.clear();
	boundingBoxListOld_.shrink_to_fit();
}
void BoundingBoxDrawer::DrawOnScene(QGraphicsScene* scene, bool useOld)
{
	if (useOld) {
		for (const QRectF& rect : boundingBoxListOld_) {
			scene->addRect(rect);
		}
	}
	else {
		for (const QRectF& rect : boundingBoxList_) {
			scene->addRect(rect);
		}
	}
}
void BoundingBoxDrawer::getBoundingBoxList(std::vector<QRectF>& list)
{
	list = boundingBoxList_;
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
	SetOldBoundingBoxes();
	boundingBoxList_ = list;
	
}

void BoundingBoxDrawer::SetOldBoundingBoxes()
{
	boundingBoxListOld_.clear();
	for (const QRectF r : boundingBoxList_) {
		boundingBoxListOld_.emplace_back(r);
	}
}
