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
	virtual void handleEmptyBoundingBox(std::shared_ptr<BoundingBoxCreator> boxCreator, std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIndex) override;
	virtual void initParameters() override;
	virtual void shiftScore(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) override;
	virtual int getRectPosMethodA(int rectListSize) override;

private:
	float t_;
	const float stepSize_; // decrease t_ by subtracting stepSize_
	const float stepFactor_; // decrease t_ by dividing by stepFactor_
	const float scoreFactor_;
	float scoreShift_;

	float sumOfIntersections_;

	int transgressions_;
	std::vector<int> rectIndicesWithTransgression_;
	int rectPos_;

	int iterations_;
	virtual void calculateOverlappingWrapper(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList);
};



template<class Data>
inline GeometryBasedOverlappingNeighbour<Data>::GeometryBasedOverlappingNeighbour(DataHolderT<Data>* data, DataHolderT<Data>* currentBest, InitialSolutionI<Data>* initSol) :
	GeometryBasedNeighbourI<Data>(data, currentBest, initSol), stepSize_(0.1), scoreFactor_(1.1), stepFactor_(2)
{
	t_ = 1;
	scoreShift_ = 0;
	iterations_ = 0;
	rectPos_ = 0;
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
	size_t bBoxListSize = 0;
	for (std::shared_ptr<BoundingBox> box : bBoxList) {
		if (!box->getRectangleIndices().empty()) ++bBoxListSize;
	}
	if (bBoxListSize == 0) {
		std::cout << "list size 0" << std::endl;
		return 0;
	}

	/*
	float average = static_cast<float>(rectListSize) / static_cast<float>(bBoxListSize);
	float boxScore = 0;

	for (std::shared_ptr<BoundingBox> box : bBoxList) {
		float rectanglesInBox = static_cast<float>((box->getRectangleIndices()).size());

		if (average > rectanglesInBox) {
			boxScore += (average - rectanglesInBox) / average;
		}

	}
	*/


	float maxOv = rectangles->size() + 1;

	this->calculateOverlappingWrapper(rectangles, bBoxList);

	// TODO: has the max score to be updated?
	// return bBoxListSize * (rectListSize + 1) - boxScore - scoreShift_;

	//std::cout << "boxList: " << bBoxListSize << ", transgressions: " << transgressions_ << ", scoreShift: " << scoreShift_ << std::endl;
	return bBoxListSize + (float)transgressions_ * scoreFactor_ - scoreShift_;
}


// count number of intersections which exceed t and sum up all intersection areas
template<class Data>
inline void GeometryBasedOverlappingNeighbour<Data>::calculateOverlappingWrapper(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList)
{
	float maxOv = rectangles->size() + 1;
	sumOfIntersections_ = 0;
	transgressions_ = 0;

	rectIndicesWithTransgression_.clear();
	rectPos_ = 0;

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
	}

	/*
	std::cout << "transgressions ";
	for (int j : rectIndicesWithTransgression_) {
		std::cout << j << ", ";
	}
	std::cout << "" << std::endl;
	*/
}


template<class Data>
inline bool GeometryBasedOverlappingNeighbour<Data>::tryFitWrapper(std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIdx, std::vector<class RectangleHolder*>* rectangles, int rectIdx, bool multipleRects)
{
	std::shared_ptr<BoundingBox>& box = boxList[boxIdx];
	std::vector<int> indices = box->getRectangleIndices();

	// float t = multipleRects ? t_ + stepSize_ : t_;
	float t = multipleRects ? t_ * stepFactor_ : t_;
	return box->tryFitOverlapping((*rectangles)[rectIdx], rectIdx, boxIdx, t, rectangles, indices, box->getBoxWidth(), box->getXPos(), box->getYPos());
}


template<class Data>
inline void GeometryBasedOverlappingNeighbour<Data>::handleEmptyBoundingBox(std::shared_ptr<BoundingBoxCreator> boxCreator, std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIndex)
{
	// IDEA: delete box but create new box after failed checking of all existing boxes
	boxCreator->resetOneBoundingBox(boxIndex);
	boxCreator->getBoundingBoxList(boxList);
}


// TODO: call this method
template<class Data>
inline void GeometryBasedOverlappingNeighbour<Data>::initParameters()
{
	std::cout << "init parameters " << std::endl;

	t_ = 1;
	scoreShift_ = 0;
	this->resetBestScore();
}

template<class Data>
inline void GeometryBasedOverlappingNeighbour<Data>::shiftScore(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList)
{
	// std::cout << "transgressions before decreasing " << transgressions_ << " t " << t_ << std::endl;
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

	}
}

template<class Data>
inline int GeometryBasedOverlappingNeighbour<Data>::getRectPosMethodA(int rectListSize)
{
	int pos;
	if (rectPos_ < rectIndicesWithTransgression_.size()) {
		pos = rectIndicesWithTransgression_[rectPos_++];
	}
	else {
		pos = rand() % rectListSize;
	}
	return pos;

}
