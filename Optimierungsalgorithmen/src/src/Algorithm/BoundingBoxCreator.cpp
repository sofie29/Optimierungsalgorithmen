#include "BoundingBoxCreator.h"
#include "BoundingBox.h"

BoundingBoxCreator::BoundingBoxCreator() {
	edgeLength_ = AlgorithmConstants::initialBoxEdgeSize_;
	optimalAmount_ = 0.0f;
	currentFitScore_ = 0.0f;
	rectSquareSize_ = 0;
}

BoundingBoxCreator::BoundingBoxCreator(const int boxLength) {
	edgeLength_ = boxLength;
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

std::shared_ptr<class BoundingBox> BoundingBoxCreator::createDeepCopy(std::shared_ptr<class BoundingBox> box) {
	std::shared_ptr<BoundingBox> firstBox = box->getFirstBox();
	std::shared_ptr<BoundingBox> secondBox = box->getSecondBox();

	if (firstBox != nullptr) {
		std::shared_ptr<BoundingBox> firstCopy = createDeepCopy(firstBox);
		std::shared_ptr<BoundingBox> secondCopy = createDeepCopy(secondBox);

		return std::make_shared<BoundingBox>
			(box->getBoxWidth(), box->getBoxHeight(), box->getXPos(), box->getYPos(), box->getRectangleIndices(), firstCopy, secondCopy);
	}
	else { // firstBox and secondBox are nullptr -> box is a leaf (can also be root node simultaneously)
		return std::make_shared<BoundingBox>
			(box->getBoxWidth(), box->getBoxHeight(), box->getXPos(), box->getYPos(), box->getRectangleIndices()); // no child boxes
	}
}

void BoundingBoxCreator::setBoundingBoxList(std::vector<std::shared_ptr<class BoundingBox>>& list)
{
	boundingBoxList_.clear();
	for (const auto& bB : list) {
		boundingBoxList_.emplace_back(createDeepCopy(bB));
		// boundingBoxList_.emplace_back(std::make_shared<BoundingBox>(edgeLength_, edgeLength_, bB->getXPos(), bB->getYPos()));
	}
}


void BoundingBoxCreator::ResetBoundingBoxList()
{


	boundingBoxList_.clear();
	rectangleList_.clear();
}

void BoundingBoxCreator::addBoundingBox(int x_pos, int y_pos, QRectF& rect, int rectIndex)
{


	boundingBoxList_.emplace_back(std::make_shared<BoundingBox>(edgeLength_, edgeLength_, x_pos, y_pos, rect, rectIndex));
	rectangleList_.emplace_back(QRectF(rect.x(), rect.y(), edgeLength_, edgeLength_));

	currentFitScore_ = (float)rectSquareSize_ / (float)(boundingBoxList_.size() * edgeLength_ * edgeLength_);
}


void BoundingBoxCreator::addBoundingBox(int x_pos, int y_pos, RectangleHolder* rectHolder, int rectIndex, int boxIndex)
{
	boundingBoxList_.emplace_back(std::make_shared<BoundingBox>(edgeLength_, edgeLength_, x_pos, y_pos, rectHolder, rectIndex, boxIndex));
	rectangleList_.emplace_back(QRectF(rectHolder->getRect().x(), rectHolder->getRect().y(), edgeLength_, edgeLength_));

	currentFitScore_ = (float)rectSquareSize_ / (float)(boundingBoxList_.size() * edgeLength_ * edgeLength_);
}


void BoundingBoxCreator::addBoundingBox(int x_pos, int y_pos, QRectF& rect)
{


	boundingBoxList_.emplace_back(std::make_shared<BoundingBox>(edgeLength_, edgeLength_, x_pos, y_pos, rect, 0)); // TODO: 0
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

void BoundingBoxCreator::OnStepDone()
{
	emit EmitRectList(rectangleList_);
	emit EmitNumberOfBoundingBoxes(boundingBoxList_.size());
	emit EmitCurrentFitScore(currentFitScore_);
}

void BoundingBoxCreator::EdgeLengthChanged(const int length) {
	edgeLength_ = length;
	RecSquareSizeChanged(rectSquareSize_);
}

void BoundingBoxCreator::resetOneBoundingBox(int index) {
	boundingBoxList_[index].reset();
	boundingBoxList_.erase(boundingBoxList_.begin() + index);
}