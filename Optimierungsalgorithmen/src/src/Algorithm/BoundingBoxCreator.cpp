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

BoundingBoxCreator::~BoundingBoxCreator()
{
	rectangleList_.clear();
	rectangleList_.shrink_to_fit();

	boundingBoxList_.clear();
	boundingBoxList_.shrink_to_fit();
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
			(box->getBoxWidth(), box->getBoxHeight(), box->getXPos(), box->getYPos(), box->getRectangleIndices(), firstCopy, secondCopy, box->getNumberOfOverlappings());
	}
	else { // firstBox and secondBox are nullptr -> box is a leaf (can also be root node simultaneously)
		return std::make_shared<BoundingBox>
			(box->getBoxWidth(), box->getBoxHeight(), box->getXPos(), box->getYPos(), box->getRectangleIndices(), box->getNumberOfOverlappings()); // no child boxes
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

void BoundingBoxCreator::RemoveEmptyBoundingBoxes()
{
	std::vector<int> deleteIdx;
	int cur_idx = 0;
	for (auto& box : boundingBoxList_) {
		if (box->getRectangleIndices().size() == 0) {
			deleteIdx.emplace_back(cur_idx);
		}
		++cur_idx;
	}
	std::reverse(deleteIdx.begin(), deleteIdx.end());
	for (int idx : deleteIdx) {
		resetOneBoundingBox(idx);
	}
	
}


void BoundingBoxCreator::addBoundingBox(int x_pos, int y_pos, RectangleHolder* rectHolder, int rectIndex, int boxIndex)
{
	boundingBoxList_.emplace_back(std::make_shared<BoundingBox>(edgeLength_, edgeLength_, x_pos, y_pos, rectHolder, rectIndex, boxIndex));
	rectangleList_.emplace_back(QRectF(rectHolder->getRect().x(), rectHolder->getRect().y(), edgeLength_, edgeLength_));

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
	rectangleList_.erase(rectangleList_.begin() + index);
	currentFitScore_ = (float)rectSquareSize_ / (float)(boundingBoxList_.size() * edgeLength_ * edgeLength_);
}

void BoundingBoxCreator::ResetBoundingBoxContent()
{
	for (auto& box : boundingBoxList_) {
		
		std::vector<int> rectIdx = box->getRectangleIndices();
		for (int idx : rectIdx) {
			box->removeRectangleIndex(idx);
		}
		box->removeLowerLevelBoundingBoxes();
	}
}
