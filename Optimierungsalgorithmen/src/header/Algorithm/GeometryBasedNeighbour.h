#pragma once
#include "NeighbourI.h"
#include "DataHolder.h"
#include "BoundingBoxCreator.h"
#include "RectangleCreator.h"
#include "InitialSolutionI.h"
#include "BoundingBox.h"

template<class Data>
class GeometryBasedNeighbour : public NeighbourI<Data>
{
public:
	GeometryBasedNeighbour(Data data, InitialSolutionI<Data>* initSol);

	virtual int optimize();

private:
	size_t rectPos = 0;
	size_t rotationIteration = 0;
	// void setSolution(std::vector<QRectF>& rectList); // temporary (should be in NeighbourI?)
	float calculateScore(size_t rectListSize, std::vector<std::shared_ptr<BoundingBox>>& bBoxList);
	bool fitBoundingBox(std::vector<int> indices, std::vector<QRectF>& rectangles, std::shared_ptr<BoundingBox>& box);
};

template<class Data>
inline GeometryBasedNeighbour<Data>::GeometryBasedNeighbour(Data data, InitialSolutionI<Data>* initSol) : NeighbourI<Data>(data, initSol) {
	//NeighbourI<Data>::data_ = data;
	rectPos = 0;
}

template<class Data>
inline int GeometryBasedNeighbour<Data>::optimize()
{
}

// Calculate the score, score should be minimized
// The score depends on the number of bounding boxes and reducing the number of rectangles in a bounding box with a small amount of rectangles is rewarded extra.
template<>
inline float GeometryBasedNeighbour<DataHolder*>::calculateScore(size_t rectListSize, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) {
	size_t bBoxListSize = bBoxList.size();

	if (bBoxListSize <= 1) {
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

/*
template<>
inline void GeometryBasedNeighbour<DataHolder*>::setSolution(std::vector<QRectF>& rectList) {
	std::shared_ptr<RectangleCreator> rectCreator = data_->getRectCreator();

	rectCreator->OnRectPositionsUpdated(rectList);
	emit OptimDone();
}
*/

template<>
inline bool GeometryBasedNeighbour<DataHolder*>::fitBoundingBox(std::vector<int> indices, std::vector<QRectF>& rectangles, std::shared_ptr<BoundingBox>& box) {
	box->removeLowerLevelBoundingBoxes();

	// place all rectangles specified in indices at the given bounding box
	for (int i : indices) {
		QRectF& rect = rectangles[i];
		int xr, yr;
		if (!box->tryFit(rect.width(), rect.height(), xr, yr)) {
			return false;
		}
		// TODO: what if changes must be reverted
		rect.moveTopLeft(QPointF(xr, yr));
	}

	return true;
}


template<>
inline int GeometryBasedNeighbour<DataHolder*>::optimize() {	

	initSol_->CreateInitialSolution(data_);

	// get bounding box list
	std::shared_ptr<BoundingBoxCreator> boxCreator = data_->getBoxCreator();
	std::vector<std::shared_ptr<BoundingBox>> bBoxList;
	boxCreator->getBoundingBoxList(bBoxList);

	// get rectangle list
	std::shared_ptr<RectangleCreator> rectCreator = data_->getRectCreator();
	std::vector<QRectF> rectList;
	rectCreator->getRectList(rectList);
	size_t rectListSize = rectList.size();

	bool foundNeighbour = false;


	/******* METHOD A : Rotate rectangle and place it at another bounding box *******/

	std::cout << "method A" << std::endl;
	
	int bBoxFromRect = 0;

	// iterate through rectangles until a rectangle is found which can be placed in a rotated way
	while (!foundNeighbour && ++rotationIteration <= rectListSize) {

		// 1) select a rectangle
		rectPos = (rectPos >= rectListSize) ? 1 : rectPos + 1;
		QRectF& rect = rectList[rectListSize - rectPos]; // get a rectangle from list, start with the last one
		bBoxFromRect = 0; // TODO (= rect.boundingBox)

		// 2) rotate the selected rectangle
		int width = rect.width();
		int height = rect.height();
		rect.setWidth(height);
		rect.setHeight(width);

		// 3) try to move to another bounding box
		int x, y;
		for (std::shared_ptr<BoundingBox> box : bBoxList) {
			if (box->tryFit(rect.width(), rect.height(), x, y)) {

				std::cout << "rects before swap: " << std::endl;
				for (int i : bBoxList[bBoxFromRect]->getRectangleIndices()) {
					std::cout << i << std::endl;
				}

				bBoxList[bBoxFromRect]->removeRectangleIndex(rectListSize - rectPos);

				rect.moveTopLeft(QPointF(x, y));

				// 4) replace all rectangles previous bounding box
				if (this->fitBoundingBox(bBoxList[bBoxFromRect]->getRectangleIndices(), rectList, bBoxList[bBoxFromRect])) {
					box->addRectangleIndex(rectListSize - rectPos);
					foundNeighbour = true;
				}
				else { // revert changes
					rect.setWidth(width);
					rect.setHeight(height);
					bBoxList[bBoxFromRect]->addRectangleIndex(rectListSize - rectPos);
					break;
				}

			}
		}
	}


	if (!foundNeighbour) { // search for another neighbour

		/******* METHOD B : Swap two rectangles *******/

		std::cout << "method B" << std::endl;

		bool isPlaced1 = false;
		bool isPlaced2 = false;
		size_t iter = 0;

		while ((!foundNeighbour || !foundNeighbour) && (++iter < 2)) {
			std::cout << "while" << iter << std::endl;


			// 1) get two rectangles
			int rectIndex1 = 0; // TODO: random
			int rectIndex2 = 1;
			QRectF& rect1 = rectList[rectIndex1];
			QRectF& rect2 = rectList[rectIndex2];

			int boxFromRect1 = 0; // TODO: rect.boundingBox
			int boxFromRect2 = 1;


			// 2) get lists of rectangles of their bounding boxes

			// tryFit with rectangles of boxFromRect1 without rect 1 and with rect2
			bBoxList[boxFromRect1]->removeRectangleIndex(rectIndex1);
			bBoxList[boxFromRect1]->addRectangleIndex(rectIndex2);
			isPlaced1 = this->fitBoundingBox(bBoxList[boxFromRect1]->getRectangleIndices(), rectList, bBoxList[boxFromRect1]);

			// tryFit with rectangles of boxFromRect2 without rect 2 and with rect1
			bBoxList[boxFromRect2]->removeRectangleIndex(rectIndex2);
			bBoxList[boxFromRect2]->addRectangleIndex(rectIndex1);
			isPlaced2 = this->fitBoundingBox(bBoxList[boxFromRect2]->getRectangleIndices(), rectList, bBoxList[boxFromRect2]);

			if (!isPlaced1 || !isPlaced2) { // revert changes
				bBoxList[boxFromRect1]->removeRectangleIndex(rectIndex2);
				bBoxList[boxFromRect1]->addRectangleIndex(rectIndex1);
				bBoxList[boxFromRect2]->removeRectangleIndex(rectIndex1);
				bBoxList[boxFromRect2]->addRectangleIndex(rectIndex2);
			}

		}

		foundNeighbour = isPlaced1 && isPlaced2;
		
	}
	
	if (!foundNeighbour) {
		std::cout << "No neighbour could be found" << std::endl;
		// return max value;
	}

	float score = this->calculateScore(rectListSize, bBoxList);

	return 0; // TODO: score FLOAT
	

	// temporary: should be called by LocalSearch class
	
	// this->setSolution(rectList); //  otherwise: reset data
}
