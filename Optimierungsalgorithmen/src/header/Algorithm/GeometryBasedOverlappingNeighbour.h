#pragma once
#include "NeighbourI.h"
#include "DataHolder.h"
#include "BoundingBoxCreator.h"
#include "RectangleCreator.h"
#include "InitialSolutionI.h"
#include "BoundingBox.h"
#include "RectangleHolder.h"
#include "GeometryBasedNeighbourI.h"

template<class Data>
class GeometryBasedOverlappingNeighbour : public GeometryBasedNeighbourI<Data> {
public:
	GeometryBasedOverlappingNeighbour(DataHolderT<Data>* data, DataHolderT<Data>* currentBest, InitialSolutionI<Data>* initSol);
	virtual float optimize() override;
	virtual float beforeScoreCalculation(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, bool isTransgressionRect) override;
	virtual bool tryFitWrapper(std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIdx, std::vector<class RectangleHolder*>* rectangles, int rectIdx, bool multipleRects) override;
	virtual void initParameters() override;
	virtual int getRectPos(const int rectListSize, std::vector<std::shared_ptr<BoundingBox>>& boxList, bool& isTransgressionRect = false) override;
	virtual int getBoxPos(const int boxListSize, std::vector<std::shared_ptr<BoundingBox>>& boxList) override;
	virtual void resetBoxPos() override;

	virtual void decreaseT(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, bool isBetter) override;
	virtual int calculateOverlappingWrapper(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) override;
	virtual void updateBoxQueue(int boxIndex1, int boxIndex2, std::vector<std::shared_ptr<BoundingBox>>& boxList, std::vector<class RectangleHolder*>* rectangles, bool deleteBox1) override;
	virtual float getScoreInformation() override;

private:
	float t_;
	const float stepSize_; // decrease t_ by subtracting stepSize_
	const float stepFactor_; // decrease t_ by dividing by stepFactor_
	const float scoreFactor_;
	float scoreShift_;

	float sumOfIntersections_;

	int transgressions_;
	int transgressionsBestSol_;
	std::vector<int> rectIndicesWithTransgression_;
	int rectPosTransgr_;
	int rectPos_;
	int rectQueuePos_;

	int boxPos_;
	std::vector<std::tuple<int, float>> boxQueue_;
	void sortBoxQueue();
	int calculateOverlappingOfOneBox(std::vector<class RectangleHolder*>* rectangles, std::shared_ptr<BoundingBox> box);
	int getAreaOfRectangles(std::shared_ptr<BoundingBox> box);
};



template<class Data>
inline GeometryBasedOverlappingNeighbour<Data>::GeometryBasedOverlappingNeighbour(DataHolderT<Data>* data, DataHolderT<Data>* currentBest, InitialSolutionI<Data>* initSol) :
	GeometryBasedNeighbourI<Data>(data, currentBest, initSol), stepSize_(0.01), scoreFactor_(1.1), stepFactor_(2)
{
	t_ = 1;
	scoreShift_ = 0;
	rectPosTransgr_ = 0;
	rectPos_ = 0;
	rectQueuePos_ = 0;
	boxPos_ = -1;

	NeighbourI<Data>::identifier_ = "GeometryOverlappingNeighbour";
}

template<>
inline float GeometryBasedOverlappingNeighbour<DataHolder*>::optimize() {
	return this->findNeighbour(false);
}

// TODO: call this method
template<>
inline void GeometryBasedOverlappingNeighbour<DataHolder*>::initParameters()
{
	t_ = 1;
	scoreShift_ = 0;
	rectPos_ = 0;
	transgressions_ = 0;
	transgressionsBestSol_ = 0;
	GeometryBasedNeighbourI<DataHolder*>::numberOfAddedBoxes_ = 0;
	GeometryBasedNeighbourI<DataHolder*>::iteration_ = 0;

	// initialize boxQueue
	boxQueue_.clear();
	std::vector<std::shared_ptr<BoundingBox>> boxList;
	data_->getData()->getBoxCreator()->getBoundingBoxList(boxList);
	for (int b = 0; b < boxList.size(); ++b) {
		boxQueue_.push_back(std::make_tuple(b, this->getAreaOfRectangles(boxList[b])));
	}
	this->sortBoxQueue();
	rectPos_ = boxList[std::get<0>(boxQueue_[0])]->getRectangleIndices().size() - 1;
	rectQueuePos_ = 0;
}


template<class Data>
inline bool GeometryBasedOverlappingNeighbour<Data>::tryFitWrapper(std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIdx, std::vector<class RectangleHolder*>* rectangles, int rectIdx, bool multipleRects)
{
	std::shared_ptr<BoundingBox>& box = boxList[boxIdx];
	std::vector<int> indices = box->getRectangleIndices();

	// float t = multipleRects ? t_ + stepSize_ : t_;
	// float t = multipleRects ? t_ * stepFactor_ : t_;
	
	float t = multipleRects ? 1 : t_; // TRANSGRESSION
	if (box->getNumberOfOverlappings() == 0 && multipleRects) t = 0;

	// std::cout << "fit rectangle " << rectIdx << " multiple: " << multipleRects << std::endl;
	bool doesFit = box->tryFitOverlapping((*rectangles)[rectIdx], rectIdx, boxIdx, t, rectangles, indices, box->getBoxWidth(), box->getXPos(), box->getYPos());
	
	return doesFit;
}

template<>
inline int GeometryBasedOverlappingNeighbour<DataHolder*>::getAreaOfRectangles(std::shared_ptr<BoundingBox> box) {
	int area = 0;
	std::vector<class RectangleHolder*>* rectList = data_->getData()->getRectCreator()->getRectList();

	for (int r : box->getRectangleIndices()) {
		QRectF& rect = (*rectList)[r]->getRectRef();
		area += rect.width() * rect.height();
	}
	return area;
}




template<class Data>
inline int GeometryBasedOverlappingNeighbour<Data>::getRectPos(const int rectListSize, std::vector<std::shared_ptr<BoundingBox>>& boxList, bool& isTransgressionRect)
{
	// the smaller t_, the more should search focus on rectangles which are causing transgressions
	int pos = rand() % rectListSize;
	int randomValue = rand() % 100 - 20; // [-20...79]
	if (randomValue < t_ * 100 || rectPosTransgr_ >= rectIndicesWithTransgression_.size()) {
		/*
		int boxIdx = std::get<0>(boxQueue_[rectQueuePos_]); // current box
		pos = boxList[boxIdx]->getRectangleIndices()[rectPos_];

		rectPos_ -= 1;
		if (rectPos_ < 0) {
			rectQueuePos_ = rectQueuePos_ >= boxQueue_.size() - 1 ? 0 : rectQueuePos_ + 1;
			int newBoxIdx = std::get<0>(boxQueue_[rectQueuePos_]); // current box
			rectPos_ = boxList[newBoxIdx]->getRectangleIndices().size() - 1;
		}
		*/
		rectPos_ = (rectPos_ >= rectListSize) ? 1 : rectPos_ + 1;
		pos = rectListSize - rectPos_;
	}
	else {
		pos = rectIndicesWithTransgression_[rectPosTransgr_++]; // select rectangle from transgression list
		isTransgressionRect = true;
	}

	return pos;
}

template<class Data>
inline int GeometryBasedOverlappingNeighbour<Data>::getBoxPos(const int boxListSize, std::vector<std::shared_ptr<BoundingBox>>& boxList)
{
	int iteration = 0;
	boxPos_ = boxPos_ < boxListSize - 1 ? boxPos_ + 1 : 0;
	while (boxList[boxPos_]->getNumberOfOverlappings() >= 3 && iteration++ < boxListSize) {
		boxPos_ = boxPos_ < boxListSize - 1 ? boxPos_ + 1 : 0;
	}
	return boxPos_;
}

template<class Data>
inline void GeometryBasedOverlappingNeighbour<Data>::resetBoxPos()
{
	boxPos_ = -1;
}



/***** Score and overlappings *****/

// count number of intersections which exceed t and sum up all intersection areas
template<class Data>
inline int GeometryBasedOverlappingNeighbour<Data>::calculateOverlappingWrapper(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList)
{
	float maxOv = -1;
	sumOfIntersections_ = 0;
	transgressions_ = 0;
	rectPosTransgr_ = 0;

	rectIndicesWithTransgression_.clear();

	for (std::shared_ptr<BoundingBox> box : bBoxList) {
		std::vector<int> indices = box->getRectangleIndices();
		int numberOfIntersectionsPerBox = 0;
		
		for (int i : indices) {
			int position = *indices.begin();
			indices.erase(indices.begin());

			QRectF& rect = (*rectangles)[position]->getRectRef();
			std::vector<int> transgressionRects;
			transgressionRects.clear();
			float intersection = box->calculateOverlappings(rectangles, indices, position, rect.x(), rect.y(), rect.width(), rect.height(), t_, transgressionRects);
			if (intersection > 0) ++numberOfIntersectionsPerBox;

			if (intersection == maxOv) {
				transgressions_ += transgressionRects.size();
				rectIndicesWithTransgression_.push_back(i);
				rectIndicesWithTransgression_.insert(rectIndicesWithTransgression_.end(), transgressionRects.begin(), transgressionRects.end());
				++numberOfIntersectionsPerBox; // count intersection twice in case there is a transgression
			}
			else {
				sumOfIntersections_ += intersection; // attention: without area of transgression and all following intersections 
			}

			if (indices.size() == 1) break;
		}
		box->setNumberOfOverlappings(numberOfIntersectionsPerBox);

	};

	rectIndicesWithTransgression_.erase(unique(rectIndicesWithTransgression_.begin(), rectIndicesWithTransgression_.end()), rectIndicesWithTransgression_.end());


	/*
	std::cout << "transgression rects ";
	for (int j : rectIndicesWithTransgression_) {
		std::cout << j << ", ";
	}
	std::cout << "" << std::endl;*/
	

	return transgressions_;
}

// Calculate score, score should be minimized
// TODO: sumOfInteraction
template<>
inline float GeometryBasedOverlappingNeighbour<DataHolder*>::beforeScoreCalculation(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, bool isTransgressionRect) {

	// allow all transgressions in old box after removing a transgression rect from it
	int transgressionsBefore_ = transgressions_;
	this->calculateOverlappingWrapper(rectangles, bBoxList);
	int diff = transgressions_ - transgressionsBefore_;
	if (isTransgressionRect && diff >= 0) { // if a transgressionRect is replaced, score must always be better
		scoreShift_ += (diff * scoreFactor_ + 1);
	}

	// upper bound of boxScore: length of rectangleList - 1
	//return ((bBoxListSize + ((float)transgressions_) * scoreFactor_ - scoreShift_) * (rectListSize - 1) - boxScore) * 0.2;
	return 0;
}

template<class Data>
inline float GeometryBasedOverlappingNeighbour<Data>::getScoreInformation()
{
	return (float)transgressions_ * scoreFactor_ - scoreShift_;
}

template<class Data>
inline void GeometryBasedOverlappingNeighbour<Data>::decreaseT(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, bool isBetter)
{
	if (isBetter) {
		transgressionsBestSol_ = transgressions_;
	}
	
	// shift score in case old box has new transgressions
	if ((GeometryBasedNeighbourI<DataHolder*>::iteration_ % 20 == 0) && t_ > 0) {
		// t_ -= stepSize_;
		// if (t_ < 0) t_ == 0;
		t_ = t_ > 0.1 ? roundf(t_ * 3/4 * 100) / 100 : 0;
		std::cout << "REDUCE T_ TO " << t_ << std::endl;

		// calculate number of transgressions after decreasing of t
		int transgressionsBefore = transgressionsBestSol_;
		this->calculateOverlappingWrapper(rectangles, bBoxList);

		// new transgressions because of t decreasing are not punished extra
		scoreShift_ += (transgressions_ - transgressionsBefore) * scoreFactor_;
	}

}

// sum up all overlapping areas within one box
// TODO: delete when it is not used
template<class Data>
inline int GeometryBasedOverlappingNeighbour<Data>::calculateOverlappingOfOneBox(std::vector<class RectangleHolder*>* rectangles, std::shared_ptr<BoundingBox> box)
{
	int sumOfIntersections = 0;

	std::vector<int> indices = box->getRectangleIndices();

	for (int i : indices) {
		indices.erase(std::remove(indices.begin(), indices.end(), i), indices.end());

		QRectF& rect = (*rectangles)[i]->getRectRef();
		float intersection = box->calculateOverlappings(rectangles, indices, i, rect.x(), rect.y(), rect.width(), rect.height(), t_);
		if (intersection > 0) sumOfIntersections += intersection; // attention: without area of transgression and all following intersections 
		if (indices.size() == 1) break;
	}

	return sumOfIntersections;
}



/***** BOX QUEUE *****/
template<class Data>
inline void GeometryBasedOverlappingNeighbour<Data>::sortBoxQueue()
{
	std::sort(
		boxQueue_.begin(),
		boxQueue_.end(),
		[](std::tuple<int, float> b1, std::tuple<int, float> b2) { return std::get<1>(b1) < std::get<1>(b2); }
	);
}

template<class Data>
inline void GeometryBasedOverlappingNeighbour<Data>::updateBoxQueue(int boxIndex1, int boxIndex2, std::vector<std::shared_ptr<BoundingBox>>& boxList, std::vector<class RectangleHolder*>* rectangles, bool deleteBox1)
{
	bool foundBox1 = false;
	bool foundBox2 = false;

	if (deleteBox1) {
		boxQueue_.erase(std::remove_if(
			boxQueue_.begin(),
			boxQueue_.end(),
			[boxIndex1](const std::tuple<int, float>& tup) { return std::get<0>(tup) == boxIndex1; }),
			boxQueue_.end());
		foundBox1 = true;
	}

	for (std::tuple<int, float>& boxTuple : boxQueue_) {
		int currentIndex = std::get<0>(boxTuple);

		if (!foundBox1 && currentIndex == boxIndex1) { // update box 1
			std::get<1>(boxTuple) = this->getAreaOfRectangles(boxList[boxIndex1]);
			foundBox1 = true;
		}

		else if (!foundBox2 && currentIndex == boxIndex2) { // update box 2
			if (deleteBox1 && boxIndex1 < boxIndex2) boxIndex2 -= 1;
			std::get<1>(boxTuple) = this->getAreaOfRectangles(boxList[boxIndex2]);
			foundBox2 = true;
		}

		if (deleteBox1 && currentIndex > boxIndex1) std::get<0>(boxTuple) = currentIndex - 1;
	}
	if (!foundBox2) {
		boxQueue_.push_back(std::make_tuple(boxIndex2, this->getAreaOfRectangles(boxList[boxIndex2])));
	}


	this->sortBoxQueue();
	rectPos_ = boxList[std::get<0>(boxQueue_[0])]->getRectangleIndices().size() - 1;
	rectQueuePos_ = 0;

	/*
	for (std::tuple<int, float> t : boxQueue_) {
		std::cout << "box " << std::get<0>(t) << ", area " << std::get<1>(t) << std::endl;
	}
	*/
	
}
