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
	virtual bool tryFitWrapper(std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIdx, std::vector<class RectangleHolder*>* rectangles, int rectIdx) override;
	virtual void handleEmptyBoundingBox(std::shared_ptr<BoundingBoxCreator> boxCreator, std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIndex) override;

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
inline bool GeometryBasedOverlappingNeighbour<Data>::tryFitWrapper(std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIdx, std::vector<class RectangleHolder*>* rectangles, int rectIdx)
{
	std::shared_ptr<BoundingBox>& box = boxList[boxIdx];
	std::vector<int> indices = box->getRectangleIndices();

	float maxOv = rectangles->size() + 1;
	float ov;
	bool reduceT = true;

	// ALTERNATIVE: define feasable solution with t
	// Reduce t when all overlappings satisfy t
	for (std::shared_ptr<BoundingBox> box : boxList) {
		std::vector<int> indices = box->getRectangleIndices();
		/*
		std::cout << "" << std::endl;
		std::cout << "check box ";

		for (int j : indices) {
			std::cout << j << ", ";
		}
		std::cout << "" << std::endl;
		*/
		for (int i : indices) {
			// indices.erase(std::remove(indices.begin(), indices.end(), i), indices.end());
			// std::cout << "check rectangle " << i << std::endl;

			QRectF& rect = (*rectangles)[i]->getRectRef();
			ov = box->calculateOverlappings(rectangles, indices, i, rect.x(), rect.y(), rect.width(), rect.height(), t_);
			// std::cout << "returned ov: " << ov << std::endl;
			if (ov == maxOv) {
				// std::cout << "returned maxOv" << std::endl;
				reduceT = false;
				break;
			}
		}
		if (!reduceT)
			break;
	}

	if (reduceT && t_ >= 0.1) {
		t_ -= 0.1;
		std::cout << "REDUCE T_ TO " << t_ << std::endl;
	}

	return box->tryFitOverlapping((*rectangles)[rectIdx], rectIdx, boxIdx, t_, rectangles, indices, box->getBoxWidth(), box->getXPos(), box->getYPos());
}

template<class Data>
inline void GeometryBasedOverlappingNeighbour<Data>::handleEmptyBoundingBox(std::shared_ptr<BoundingBoxCreator> boxCreator, std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIndex)
{
	// IDEA: set boundingbox at end of boxlist

}
