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
	virtual float calculateScore(size_t rectListSize, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) override;
	virtual bool tryFitWrapper(std::shared_ptr<BoundingBox>& box, RectangleHolder* rectHolder, int boxIdx) override;
};


template<class Data>
inline GeometryBasedNeighbour<Data>::GeometryBasedNeighbour(DataHolderT<Data>* data, DataHolderT<Data>* currentBest, InitialSolutionI<Data>* initSol) : GeometryBasedNeighbourI<Data>(data, currentBest, initSol) {
}

template<>
inline float GeometryBasedNeighbour<DataHolder*>::optimize() {
	return this->findNeighbour(true, true);
}

// Calculate score, score should be minimized
// The score depends on the number of bounding boxes and reducing the number of rectangles in a bounding box with a small amount of rectangles is rewarded extra.
template<>
inline float GeometryBasedNeighbour<DataHolder*>::calculateScore(size_t rectListSize, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) {
	size_t bBoxListSize = bBoxList.size();
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

template<class Data>
inline bool GeometryBasedNeighbour<Data>::tryFitWrapper(std::shared_ptr<BoundingBox>& box, RectangleHolder* rectHolder, int boxIdx)
{
	return box->tryFit(rectHolder, boxIdx);
}
