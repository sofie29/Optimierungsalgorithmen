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
	virtual float calculateScore(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) override;
	virtual bool tryFitWrapper(std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIdx, std::vector<class RectangleHolder*>* rectangles, int rectIdx, bool multipleRects) override;
	virtual void initParameters() override;
	virtual int getRectPos(const int rectListSize, std::vector<std::shared_ptr<BoundingBox>>& boxList, bool& isTransgressionRect = false) override;
	virtual int getBoxPos(const int boxListSize) override;
	virtual void resetBoxPos() override;

	virtual void shiftScore(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) override;
	virtual int calculateOverlappingWrapper(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) override;
	virtual void updateBoxQueue(int boxIndex1, int boxIndex2, std::vector<std::shared_ptr<BoundingBox>>& boxList, std::vector<class RectangleHolder*>* rectangles, bool deleteBox1) override;


private:
	float t_;
	const float stepSize_; // decrease t_ by subtracting stepSize_
	const float stepFactor_; // decrease t_ by dividing by stepFactor_
	const float scoreFactor_;
	float scoreShift_;

	float sumOfIntersections_;

	int transgressions_;
	std::vector<int> rectIndicesWithTransgression_;
	int rectPosTransgr_;
	int rectPos_;
	int rectQueuePos_;

	int boxPos_;
	std::vector<int> visitedBoxes_;
	std::vector<std::tuple<int, float>> boxQueue_;
	void sortBoxQueue();

	int iterations_;

};



template<class Data>
inline GeometryBasedOverlappingNeighbour<Data>::GeometryBasedOverlappingNeighbour(DataHolderT<Data>* data, DataHolderT<Data>* currentBest, InitialSolutionI<Data>* initSol) :
	GeometryBasedNeighbourI<Data>(data, currentBest, initSol), stepSize_(0.1), scoreFactor_(1.1), stepFactor_(2)
{
	t_ = 1;
	scoreShift_ = 0;
	iterations_ = 0;
	rectPosTransgr_ = 0;
	rectPos_ = 0;
	rectQueuePos_ = 0;
	boxPos_ = -1;
}

template<>
inline float GeometryBasedOverlappingNeighbour<DataHolder*>::optimize() {
	return this->findNeighbour(false);
}

// Calculate score, score should be minimized
// TODO: sumOfInteraction
template<>
inline float GeometryBasedOverlappingNeighbour<DataHolder*>::calculateScore(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) {
	++iterations_;
	size_t rectListSize = rectangles->size();
	size_t bBoxListSize = bBoxList.size();
	if (bBoxListSize == 0) {
		std::cout << "list size 0" << std::endl;
		return 0;
	}

	
	float average = static_cast<float>(rectListSize) / static_cast<float>(bBoxListSize);
	float boxScore = 0;

	for (std::shared_ptr<BoundingBox> box : bBoxList) {
		float rectanglesInBox = static_cast<float>((box->getRectangleIndices()).size());

		if (average > rectanglesInBox) {
			boxScore += (average - rectanglesInBox) / average;
		}

	}

	float maxOv = rectangles->size() + 1;
	this->calculateOverlappingWrapper(rectangles, bBoxList);

	// upper bound of boxScore: length of rectangleList - 1
	//std::cout << "boxList: " << bBoxListSize << ", transgressions: " << transgressions_ << ", scoreShift: " << scoreShift_ << std::endl;
	return (bBoxListSize + (float)transgressions_ * scoreFactor_ - scoreShift_) * (rectListSize - 1) - boxScore;
}


template<class Data>
inline bool GeometryBasedOverlappingNeighbour<Data>::tryFitWrapper(std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIdx, std::vector<class RectangleHolder*>* rectangles, int rectIdx, bool multipleRects)
{
	std::shared_ptr<BoundingBox>& box = boxList[boxIdx];
	std::vector<int> indices = box->getRectangleIndices();

	// float t = multipleRects ? t_ + stepSize_ : t_;
	float t = multipleRects ? t_ * stepFactor_ : t_;

	// std::cout << "fit rectangle " << rectIdx << " multiple: " << multipleRects << std::endl;
	return box->tryFitOverlapping((*rectangles)[rectIdx], rectIdx, boxIdx, t, rectangles, indices, box->getBoxWidth(), box->getXPos(), box->getYPos());
}



// TODO: call this method
template<>
inline void GeometryBasedOverlappingNeighbour<DataHolder*>::initParameters()
{
	t_ = 1;
	scoreShift_ = 0;
	rectPos_ = 0;

	this->resetBestScore();

	// reset boxQueue
	boxQueue_.clear();
	std::vector<std::shared_ptr<BoundingBox>> boxList;
	data_->getData()->getBoxCreator()->getBoundingBoxList(boxList);
	std::cout << boxList.size() << std::endl;
	for (int i = 0; i < boxList.size(); ++i) {
		// struct with box index und filled area TODO AREA
		boxQueue_.push_back(std::make_tuple(i, boxList[i]->getRectangleIndices().size()));
	}
	this->sortBoxQueue();
	rectPos_ = boxList[0]->getRectangleIndices().size() - 1;
	rectQueuePos_ = 0;
}


template<class Data>
inline void GeometryBasedOverlappingNeighbour<Data>::shiftScore(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList)
{
	// std::cout << "transgressions before decreasing " << transgressions_ << ", t " << t_ << std::endl;
	// if (transgressions_ == 0 && t_ >= stepSize_) {
	if ((transgressions_ == 0 || iterations_ % 10 == 0) && t_ != 0 ) {
		// t_ -= stepSize_;
		t_ = t_ > 0.1 ? t_ / 2 : 0; // 1, 0.5, 0.25, 0.125, 0.0625, 0
		std::cout << "REDUCE T_ TO " << t_ << std::endl;

		// calculate number of transgressions after decreasing of t
		this->calculateOverlappingWrapper(rectangles, bBoxList);
		// std::cout << "transgressions after decreasing " << transgressions_ << " t " << t_ << std::endl;

		scoreShift_ += transgressions_ * scoreFactor_;
		// std::cout << "new score shift " << scoreShift_ << std::endl;

		// TODO: score shift nur mit differenz der transgressions, falls transgressions nicht 0 sind
	}
}

// count number of intersections which exceed t and sum up all intersection areas
template<class Data>
inline int GeometryBasedOverlappingNeighbour<Data>::calculateOverlappingWrapper(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList)
{
	float maxOv = rectangles->size() + 1;
	sumOfIntersections_ = 0;
	transgressions_ = 0;

	rectIndicesWithTransgression_.clear();
	rectPosTransgr_ = 0;

	for (std::shared_ptr<BoundingBox> box : bBoxList) {
		std::vector<int> indices = box->getRectangleIndices();

		for (int i : indices) {
			indices.erase(std::remove(indices.begin(), indices.end(), i), indices.end());

			QRectF& rect = (*rectangles)[i]->getRectRef();
			float intersection = box->calculateOverlappings(rectangles, indices, i, rect.x(), rect.y(), rect.width(), rect.height(), t_);
			sumOfIntersections_ += intersection; // attention: maxOv is added
			if (intersection == maxOv) {
				++transgressions_;
				rectIndicesWithTransgression_.push_back(i); // other rectangle is missing here
			}

			if (indices.size() == 1) break;
		}
	};

	/*
	std::cout << "transgressions ";
	for (int j : rectIndicesWithTransgression_) {
		std::cout << j << ", ";
	}
	std::cout << "" << std::endl;
	*/
	return transgressions_;
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
		// std::cout << "try box " << boxIdx << " with rect: " << pos << std::endl;

		rectPos_ -= 1;
		if (rectPos_ < 0) {
			rectQueuePos_ = rectQueuePos_ >= boxQueue_.size() - 1 ? 0 : rectQueuePos_ + 1;
			int newBoxIdx = std::get<0>(boxQueue_[rectQueuePos_]); // current box
			rectPos_ = boxList[newBoxIdx]->getRectangleIndices().size() - 1;
		}
		// std::cout << "new rectQueueIdx " << rectQueuePos_ << ", new rectPos: " << rectPos_ << std::endl;
		*/
	}
	else {
		pos = rectIndicesWithTransgression_[rectPosTransgr_++]; // select rectangle from transgression list
		isTransgressionRect = true;
		//std::cout << "transgression rect: " << pos << std::endl;
	}

	return pos;
}

template<class Data>
inline int GeometryBasedOverlappingNeighbour<Data>::getBoxPos(const int boxListSize)
{
	boxPos_ = rand() % boxListSize;
	// with 90 % probability, get another index in case the index was already visited
	while (rand() % 100 < 90 && std::find(visitedBoxes_.begin(), visitedBoxes_.end(), boxPos_) != visitedBoxes_.end()) {
		boxPos_ = rand() % boxListSize;
	}
	visitedBoxes_.push_back(boxPos_);
	return boxPos_;
}

template<class Data>
inline void GeometryBasedOverlappingNeighbour<Data>::resetBoxPos()
{
	boxPos_ = -1;
	visitedBoxes_.clear();
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

		if (!foundBox1 && currentIndex == boxIndex1) {
			// update box 1
			std::get<1>(boxTuple) = boxList[boxIndex1]->getRectangleIndices().size();
			foundBox1 = true;
		}
		else if (!foundBox2 && currentIndex == boxIndex2) {
			// update box 2
			if (deleteBox1 && boxIndex1 < boxIndex2) boxIndex2 -= 1;
			std::get<1>(boxTuple) = boxList[boxIndex2]->getRectangleIndices().size();
			foundBox2 = true;
		}

		if (deleteBox1 && currentIndex > boxIndex1) std::get<0>(boxTuple) = currentIndex - 1;
	}

	// TODO: change 1 here
	if (!foundBox2) {
		boxQueue_.push_back(std::make_tuple(boxIndex2, 1.0));
	}

	this->sortBoxQueue();
	rectPos_ = boxList[0]->getRectangleIndices().size() - 1;
	rectQueuePos_ = 0;

	/*
	for (std::tuple<int, float> t : boxQueue_) {
		std::cout << "box " << std::get<0>(t) << "rects " << std::get<1>(t) << std::endl;
	}
	*/
}
