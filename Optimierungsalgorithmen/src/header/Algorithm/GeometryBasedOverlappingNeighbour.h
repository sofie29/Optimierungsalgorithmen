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
	virtual float calculateScore(size_t rectListSize, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) override;
	virtual bool tryFitWrapper(std::shared_ptr<BoundingBox>& box, int rectIdx, int boxIdx, std::vector<class RectangleHolder*>* rectangles) override;
private:
	float t_;
};



template<class Data>
inline GeometryBasedOverlappingNeighbour<Data>::GeometryBasedOverlappingNeighbour(DataHolderT<Data>* data, DataHolderT<Data>* currentBest, InitialSolutionI<Data>* initSol) : GeometryBasedNeighbourI<Data>(data, currentBest, initSol) {
	t_ = 1;
}

template<>
inline float GeometryBasedOverlappingNeighbour<DataHolder*>::optimize() {
	return this->findNeighbour(true, false);
}

// Calculate score, score should be minimized
// TODO
template<>
inline float GeometryBasedOverlappingNeighbour<DataHolder*>::calculateScore(size_t rectListSize, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) {
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
inline bool GeometryBasedOverlappingNeighbour<Data>::tryFitWrapper(std::shared_ptr<BoundingBox>& box, int rectIdx, int boxIdx, std::vector<class RectangleHolder*>* rectangles)
{
	std::vector<int> indices = box->getRectangleIndices();
	indices.erase(std::remove(indices.begin(), indices.end(), rectIdx), indices.end());
	if (t_ >= 0.1) {
		t_ -= 0.1;
	}

	return box->tryFitOverlapping((*rectangles)[rectIdx], boxIdx, t_, rectangles, indices, box->getBoxWidth(), box->getXPos(), box->getYPos());
}
