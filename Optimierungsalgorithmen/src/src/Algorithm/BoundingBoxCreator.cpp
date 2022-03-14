#include "BoundingBoxCreator.h"
#include "BoundingBox.h"

BoundingBoxCreator::BoundingBoxCreator() {
	edgeLength_ = AlgorithmConstants::initialBoxEdgeSize_;
}
void BoundingBoxCreator::getRectList(std::vector<QRectF>& list)
{
	list = rectangleList_;
}

void BoundingBoxCreator::getBoundingBoxList(std::vector<std::shared_ptr<class BoundingBox>>& list)
{
	list = boundingBoxList_;
}

void BoundingBoxCreator::addBoundingBox(int x_pos, int y_pos, QRectF& rect)
{

	boundingBoxList_.emplace_back(std::make_unique<BoundingBox>(edgeLength_, edgeLength_, x_pos, y_pos, rect));
	rectangleList_.emplace_back(QRectF(rect.x(), rect.y(), edgeLength_, edgeLength_));

	
}

void BoundingBoxCreator::OnOptimDone()
{
	emit EmitRectList(rectangleList_);
	boundingBoxList_.clear();
	rectangleList_.clear();
}

void BoundingBoxCreator::EdgeLengthChanged(const int length) {
	edgeLength_ = length;
}