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
class GeometryBasedNeighbour : public GeometryBasedNeighbourI<Data> {
public:
	GeometryBasedNeighbour(DataHolderT<Data>* data, DataHolderT<Data>* currentBest, InitialSolutionI<Data>* initSol);
	virtual float optimize() override;
	virtual float calculateScore(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) override;
	virtual bool tryFitWrapper(std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIdx, std::vector<class RectangleHolder*>* rectangles, int rectIdx, bool multipleRects) override;
	virtual void handleEmptyBoundingBox(std::shared_ptr<BoundingBoxCreator> boxCreator, std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIndex) override;
	virtual void initParameters() override;
	virtual void shiftScore(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) override;
	virtual int getRectPosMethodA(int rectListSize) override;

private:
	int rectPos_;
	virtual float calculateScoreDependingOnRectangles(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList);
	virtual float calculateScoreDependingOnArea(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList);
};


template<class Data>
inline GeometryBasedNeighbour<Data>::GeometryBasedNeighbour(DataHolderT<Data>* data, DataHolderT<Data>* currentBest, InitialSolutionI<Data>* initSol) : GeometryBasedNeighbourI<Data>(data, currentBest, initSol)
{
	rectPos_ = 0;
}


template<>
inline float GeometryBasedNeighbour<DataHolder*>::optimize() {
	return this->findNeighbour(true);
}


template<>
inline float GeometryBasedNeighbour<DataHolder*>::calculateScore(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) {
	return calculateScoreDependingOnRectangles(rectangles, bBoxList);
	// return calculateScoreDependingOnArea(rectangles, bBoxList);
}


// Calculate score, score should be minimized
// The score depends on the number of bounding boxes and reducing the number of rectangles in a bounding box with a small amount of rectangles is rewarded extra.
template<>
inline float GeometryBasedNeighbour<DataHolder*>::calculateScoreDependingOnRectangles(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) {
	size_t bBoxListSize = bBoxList.size();
	size_t rectListSize = rectangles->size();

	if (bBoxListSize <= 1) {
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

	return bBoxListSize * (rectListSize - 1) - boxScore;
}


// Calculate score, score should be minimized
// The score depends on the number of bounding boxes and reducing the number of rectangles in a bounding box with a small amount of rectangles is rewarded extra.
template<>
inline float GeometryBasedNeighbour<DataHolder*>::calculateScoreDependingOnArea(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) {
	size_t bBoxListSize = bBoxList.size();
	size_t rectListSize = rectangles->size();
	if (bBoxListSize <= 0) return 0;
	float boxLength = bBoxList[0]->getBoxWidth();
	float boxArea = boxLength * boxLength;

	if (bBoxListSize <= 1) {
		std::cout << "list size 0" << std::endl;
		return 0;
	}

	float average = 0.5;
	float boxScore = 0;

	for (std::shared_ptr<BoundingBox> box : bBoxList) {
		float usedArea = 0;
		for (RectangleHolder* rect : *rectangles) {
			usedArea += (float)(rect->getRectRef().width() * rect->getRectRef().height());
		}
		float usedAreaPercent = usedArea / boxArea;
		if (average > usedArea) {
			boxScore += (average - usedArea) / average;
		}

	}
	// maximaler Boxscore: 1 * rectangles
	// std::cout << bBoxListSize << " " << boxScore << std::endl;
	return bBoxListSize * (rectListSize + 1) - boxScore;
}


template<class Data>
inline bool GeometryBasedNeighbour<Data>::tryFitWrapper(std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIdx, std::vector<class RectangleHolder*>* rectangles, int rectIdx, bool multipleRects)
{
	return boxList[boxIdx]->tryFit((*rectangles)[rectIdx], boxIdx);
}


template<class Data>
inline void GeometryBasedNeighbour<Data>::handleEmptyBoundingBox(std::shared_ptr<BoundingBoxCreator> boxCreator, std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIndex)
{
	boxCreator->resetOneBoundingBox(boxIndex);
	boxCreator->getBoundingBoxList(boxList);
}

template<class Data>
inline void GeometryBasedNeighbour<Data>::initParameters()
{
	this->resetBestScore();
}

template<class Data>
inline void GeometryBasedNeighbour<Data>::shiftScore(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList)
{
}

template<class Data>
inline int GeometryBasedNeighbour<Data>::getRectPosMethodA(int rectListSize)
{
	rectPos_ = (rectPos_ >= rectListSize) ? 1 : rectPos_ + 1;
	return rectListSize - rectPos_;
}
