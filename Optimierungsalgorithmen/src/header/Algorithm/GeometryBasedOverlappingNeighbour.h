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
	virtual void beforeScoreCalculation(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, bool isTransgressionRect) override;
	virtual bool tryFitWrapper(std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIdx, std::vector<class RectangleHolder*>* rectangles, int rectIdx, bool multipleRects) override;
	virtual void initParameters() override;
	virtual int getRectPos(const int rectListSize, std::vector<std::shared_ptr<BoundingBox>>& boxList, bool& isTransgressionRect = false) override;
	virtual int getBoxPos(const int boxListSize, std::vector<std::shared_ptr<BoundingBox>>& boxList) override;
	virtual void resetBoxPos() override;

	virtual void decreaseT(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, bool isBetter) override;
	virtual int calculateOverlappingWrapper(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) override;
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

	int boxPos_;

	float lastTDecreasingIteration_;
	bool wasFitScoreLowerThan1_;
};



template<class Data>
inline GeometryBasedOverlappingNeighbour<Data>::GeometryBasedOverlappingNeighbour(DataHolderT<Data>* data, DataHolderT<Data>* currentBest, InitialSolutionI<Data>* initSol) :
	GeometryBasedNeighbourI<Data>(data, currentBest, initSol), stepSize_(0.01), scoreFactor_(1.1), stepFactor_(2)
{
	t_ = 1;
	scoreShift_ = 0;
	rectPosTransgr_ = 0;
	rectPos_ = 0;
	boxPos_ = -1;
	lastTDecreasingIteration_ = 0;
	wasFitScoreLowerThan1_ = true; // is true, when fitScore was lower than 1 after fitScore was higher than 1

	NeighbourI<Data>::identifier_ = "GeometryOverlappingNeighbour";
}

template<>
inline float GeometryBasedOverlappingNeighbour<DataHolder*>::optimize() {
	return this->findNeighbour(false);
}


template<>
inline void GeometryBasedOverlappingNeighbour<DataHolder*>::initParameters()
{
	t_ = 1;
	scoreShift_ = 0;
	rectPos_ = 0;
	transgressions_ = 0;
	transgressionsBestSol_ = 0;
	lastTDecreasingIteration_ = 0;
	wasFitScoreLowerThan1_ = true;

	GeometryBasedNeighbourI<DataHolder*>::numberOfAddedBoxes_ = 0;
	GeometryBasedNeighbourI<DataHolder*>::iteration_ = 0;

}


template<class Data>
inline bool GeometryBasedOverlappingNeighbour<Data>::tryFitWrapper(std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIdx, std::vector<class RectangleHolder*>* rectangles, int rectIdx, bool multipleRects)
{
	std::shared_ptr<BoundingBox>& box = boxList[boxIdx];
	std::vector<int> indices = box->getRectangleIndices();
	
	float t = multipleRects ? 1 : t_;
	if (box->getNumberOfOverlappings() == 0 && multipleRects) t = 0;

	return box->tryFitOverlapping((*rectangles)[rectIdx], rectIdx, boxIdx, t, rectangles, indices, box->getBoxWidth(), box->getXPos(), box->getYPos());
}




template<class Data>
inline int GeometryBasedOverlappingNeighbour<Data>::getRectPos(const int rectListSize, std::vector<std::shared_ptr<BoundingBox>>& boxList, bool& isTransgressionRect)
{
	// the smaller t_, the more should search focus on rectangles which are causing transgressions
	int pos = 0;
	int randomValue = rand() % 100 - 20; // [-20...79]
	// t_ = 1: mit 80 % Wsk. Rechteck der Reihenfolge nutzen (20 % Transgression-Rechteck)
	// t_ = 0: mit 20 % Wsk. Rechteck der Reihenfolge nutzen (80 % Transgression-Rechteck)
	if (randomValue < t_ * 100 || rectPosTransgr_ >= rectIndicesWithTransgression_.size()) {
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
				sumOfIntersections_ += intersection;
			}

			if (indices.size() == 1) break;
		}
		box->setNumberOfOverlappings(numberOfIntersectionsPerBox);

	};

	rectIndicesWithTransgression_.erase(unique(rectIndicesWithTransgression_.begin(), rectIndicesWithTransgression_.end()), rectIndicesWithTransgression_.end());	

	return transgressions_;
}

// Calculate score, score should be minimized, without sumOfInteraction
template<>
inline void GeometryBasedOverlappingNeighbour<DataHolder*>::beforeScoreCalculation(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, bool isTransgressionRect) {

	// allow all transgressions in old box after removing a transgression rect from it
	int transgressionsBefore_ = transgressions_;
	this->calculateOverlappingWrapper(rectangles, bBoxList);
	int diff = transgressions_ - transgressionsBefore_;
	if (isTransgressionRect && diff >= 0) { // if a transgressionRect is replaced, score must always be better
		scoreShift_ += (diff * scoreFactor_ + 1);
	}
}

template<class Data>
inline float GeometryBasedOverlappingNeighbour<Data>::getScoreInformation()
{
	return (float)transgressions_ * scoreFactor_ - scoreShift_;
}

template<>
inline void GeometryBasedOverlappingNeighbour<DataHolder*>::decreaseT(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, bool isBetter)
{
	if (isBetter) {
		transgressionsBestSol_ = transgressions_;
	}
	if (t_ == 0) {
		return;
	}

	float fitScore = data_->getData()->getBoxCreator()->getCurrentFitScore();
	if (fitScore < 1) {
		wasFitScoreLowerThan1_ = true;
	}
	// std::cout << "wasLowerThan1: " << wasFitScoreLowerThan1_ << ", lastDecreasingIteration: " << lastTDecreasingIteration_ << ", noBetterNeighbourFound: " << noBetterNeighbourFound_ << std::endl;
	if ((fitScore > 1 && wasFitScoreLowerThan1_) || lastTDecreasingIteration_ + 20 == iteration_ || noBetterNeighbourFound_ >= rectangles->size()*1/6) {
		// Reduce t:
		t_ = t_ > 0.05 ? roundf(t_ * 3 / 4 * 100) / 100 : 0; // 11 different t_ before t_ is 0
		lastTDecreasingIteration_ = iteration_;
		wasFitScoreLowerThan1_ = false;

		std::cout << "REDUCE T_ TO " << t_ << std::endl;

		// Shift score in case old box has new transgressions:

		// calculate number of transgressions after decreasing of t
		int transgressionsBefore = transgressionsBestSol_;
		this->calculateOverlappingWrapper(rectangles, bBoxList);

		// new transgressions because of t decreasing are not punished extra
		scoreShift_ += (transgressions_ - transgressionsBefore) * scoreFactor_;

		if (transgressions_ - transgressionsBefore == 0) wasFitScoreLowerThan1_ = true;
	}

	/*
	// shift score in case old box has new transgressions
	if ((GeometryBasedNeighbourI<DataHolder*>::iteration_ % 20 == 0) && t_ > 0) {
		t_ = t_ > 0.1 ? roundf(t_ * 3/4 * 100) / 100 : 0;
		std::cout << "REDUCE T_ TO " << t_ << std::endl;

		// calculate number of transgressions after decreasing of t
		int transgressionsBefore = transgressionsBestSol_;
		this->calculateOverlappingWrapper(rectangles, bBoxList);

		// new transgressions because of t decreasing are not punished extra
		scoreShift_ += (transgressions_ - transgressionsBefore) * scoreFactor_;
	}
	*/

}
