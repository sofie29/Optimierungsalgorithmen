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
	virtual bool tryFitWrapper(std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIdx, std::vector<class RectangleHolder*>* rectangles, int rectIdx) override;
	virtual void handleEmptyBoundingBox(std::shared_ptr<BoundingBoxCreator> boxCreator, std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIndex) override;
	virtual void initParameters() override;

private:
	float t_;
	float scoreShift_;
	float scoreFactor_;
	virtual void calculateOverlappingWrapper(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, int& transgressions, float& sumOfIntersections);
};



template<class Data>
inline GeometryBasedOverlappingNeighbour<Data>::GeometryBasedOverlappingNeighbour(DataHolderT<Data>* data, DataHolderT<Data>* currentBest, InitialSolutionI<Data>* initSol) : GeometryBasedNeighbourI<Data>(data, currentBest, initSol) {
	t_ = 1;
	scoreShift_ = 0;
	scoreFactor_ = 1.01;
}

template<>
inline float GeometryBasedOverlappingNeighbour<DataHolder*>::optimize() {
	return this->findNeighbour(true, false);
}

// Calculate score, score should be minimized
// TODO
template<>
inline float GeometryBasedOverlappingNeighbour<DataHolder*>::calculateScore(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) {
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


	float maxOv = rectangles->size() + 1;
	float sumOfIntersections;
	int transgressions;

	this->calculateOverlappingWrapper(rectangles, bBoxList, transgressions, sumOfIntersections);

	// TODO: only update when score is better
	if (transgressions == 0 && t_ >= 0.1) {
		t_ -= 0.1;
		std::cout << "REDUCE T_ TO " << t_ << std::endl;

		// number of rectangles is the maximum number of overlappings
		scoreShift_ += rectangles->size() * scoreFactor_;
	}

	// TODO: has the max score to be updated?
	// return bBoxListSize * (rectListSize + 1) - boxScore - scoreShift_;
	return bBoxListSize + transgressions * scoreFactor_ - scoreShift_;
}


// count number of intersections which exceed t and sum up all intersection areas
template<class Data>
inline void GeometryBasedOverlappingNeighbour<Data>::calculateOverlappingWrapper(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, int& transgressions, float& sumOfIntersections)
{
	float maxOv = rectangles->size() + 1;
	sumOfIntersections = 0;
	transgressions = 0;

	for (std::shared_ptr<BoundingBox> box : bBoxList) {
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
			indices.erase(std::remove(indices.begin(), indices.end(), i), indices.end());

			/*
			std::cout << "check rectangle " << i << " with ";
			for (int j : indices) {
				std::cout << j << ", ";
			}
			std::cout << "" << std::endl;
			*/

			QRectF& rect = (*rectangles)[i]->getRectRef();
			float intersection = box->calculateOverlappings(rectangles, indices, i, rect.x(), rect.y(), rect.width(), rect.height(), t_);
			sumOfIntersections += intersection; // attention: maxOv is added
			if (intersection == maxOv) ++transgressions;

			if (indices.size() == 1) break;
		}
	}
}


template<class Data>
inline bool GeometryBasedOverlappingNeighbour<Data>::tryFitWrapper(std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIdx, std::vector<class RectangleHolder*>* rectangles, int rectIdx)
{
	std::shared_ptr<BoundingBox>& box = boxList[boxIdx];
	std::vector<int> indices = box->getRectangleIndices();

	return box->tryFitOverlapping((*rectangles)[rectIdx], rectIdx, boxIdx, t_, rectangles, indices, box->getBoxWidth(), box->getXPos(), box->getYPos());
}


template<class Data>
inline void GeometryBasedOverlappingNeighbour<Data>::handleEmptyBoundingBox(std::shared_ptr<BoundingBoxCreator> boxCreator, std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIndex)
{
	// IDEA: set boundingbox at end of boxlist

}

template<class Data>
inline void GeometryBasedOverlappingNeighbour<Data>::initParameters()
{
	t_ = 1;
	scoreShift_ = 0;
}
