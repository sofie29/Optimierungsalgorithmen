#include "BoundingBoxCreator.h"
#include "BoundingBox.h"

BoundingBoxCreator::BoundingBoxCreator() {
	edgeLength_ = AlgorithmConstants::initialBoxEdgeSize_;
	optimalAmount_ = 0.0f;
	currentFitScore_ = 0.0f;
	rectSquareSize_ = 0;
}


void BoundingBoxCreator::getRectList(std::vector<QRectF>& list)
{
	list = rectangleList_;
}

void BoundingBoxCreator::setRectList(std::vector<QRectF>& list)
{
	rectangleList_.clear();
	for (const auto& rect : list) {
		rectangleList_.emplace_back(QRectF(rect.x(), rect.y(), edgeLength_, edgeLength_));
	}
}

void BoundingBoxCreator::getBoundingBoxList(std::vector<std::shared_ptr<class BoundingBox>>& list)
{
	list = boundingBoxList_;
}

void BoundingBoxCreator::setBoundingBoxList(std::vector<std::shared_ptr<class BoundingBox>>& list)
{
	boundingBoxList_.clear();
	for (const auto& bB : list) {
		boundingBoxList_.emplace_back(std::make_shared<BoundingBox>(edgeLength_, edgeLength_, bB->getXPos(), bB->getYPos()));
	}
	
}

void BoundingBoxCreator::ResetBoundingBoxList()
{
	
	currentFitScore_ = 0.0f;
	boundingBoxList_.clear();
	rectangleList_.clear();
}

void BoundingBoxCreator::addBoundingBox(int x_pos, int y_pos, QRectF& rect)
{
	

	boundingBoxList_.emplace_back(std::make_shared<BoundingBox>(edgeLength_, edgeLength_, x_pos, y_pos, rect));
	rectangleList_.emplace_back(QRectF(rect.x(), rect.y(), edgeLength_, edgeLength_));

	currentFitScore_ = (float)rectSquareSize_ / (float)(boundingBoxList_.size() * edgeLength_ * edgeLength_);
}

float BoundingBoxCreator::getCurrentFitScore()
{
	return currentFitScore_;
}

void BoundingBoxCreator::setCurrentFitScore(float score)
{
	currentFitScore_ = score;
}


void BoundingBoxCreator::RecSquareSizeChanged(int rectSquareSize)
{
	rectSquareSize_ = rectSquareSize;
	optimalAmount_ = (float)rectSquareSize_ / (float)(edgeLength_ * edgeLength_);
	emit EmitOptimalAmountRect(optimalAmount_);
}

void BoundingBoxCreator::OnOptimDone()
{
	emit EmitRectList(rectangleList_);
	emit EmitNumberOfBoundingBoxes(boundingBoxList_.size());
	emit EmitCurrentFitScore(currentFitScore_);
	boundingBoxList_.clear();
	rectangleList_.clear();
}

void BoundingBoxCreator::EdgeLengthChanged(const int length) {
	edgeLength_ = length;
	RecSquareSizeChanged(rectSquareSize_);
}