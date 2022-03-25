#pragma once
#include "NeighbourI.h"
#include "DataHolder.h"
#include "BoundingBoxCreator.h"
#include "RectangleCreator.h"
#include "InitialSolutionI.h"
#include "BoundingBox.h"
#include "RectangleHolder.h"
template<class Data>
class GeometryBasedNeighbour : public NeighbourI<Data> {
public:
	GeometryBasedNeighbour(Data data, Data currentBest, InitialSolutionI<Data>* initSol);

	virtual float optimize();

private:
	size_t rectPos;
	float calculateScore(size_t rectListSize, std::vector<std::shared_ptr<BoundingBox>>& bBoxList);
	float bestScore_;
	bool isFirstIteration_;
	bool fitBoundingBox(std::vector<int> indices, std::vector<class RectangleHolder*>* rectangles, std::shared_ptr<BoundingBox>& box, int boundingBoxIndex);
	bool removeRectFromBox(std::shared_ptr<BoundingBox>& newBox, std::shared_ptr<BoundingBox>& oldBox, int oldBoxIdx, int rectIdx, int newBoxIdx, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, std::vector<class RectangleHolder*>* rectList);


};

template<class Data>
inline GeometryBasedNeighbour<Data>::GeometryBasedNeighbour(Data data, Data currentBest, InitialSolutionI<Data>* initSol) : NeighbourI<Data>(data, currentBest, initSol) {
	rectPos = 0;
	bestScore_ = AlgorithmConstants::maxScore;
	isFirstIteration_ = true;
}

template<class Data>
inline float GeometryBasedNeighbour<Data>::optimize()
{
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

// attention: this method doesn't update index vector of the bounding box
template<>
inline bool GeometryBasedNeighbour<DataHolder*>::fitBoundingBox(std::vector<int> indices, std::vector<class RectangleHolder*>* rectangles, std::shared_ptr<BoundingBox>& box, int boundingBoxIndex) {
	box->removeLowerLevelBoundingBoxes();

	// place all rectangles specified in indices at the given bounding box
	for (int i : indices) {
		if (!box->tryFit((*rectangles)[i], boundingBoxIndex)) {
			return false;
		}
	}
	return true;
}

template<>
inline bool GeometryBasedNeighbour<DataHolder*>::removeRectFromBox(std::shared_ptr<BoundingBox>&newBox, std::shared_ptr<BoundingBox>&oldBox, int oldBoxIdx, int rectIdx, int newBoxIdx, std::vector<std::shared_ptr<BoundingBox>>&bBoxList, std::vector<class RectangleHolder*>*rectList) {
	std::vector<int> newIndices = oldBox->getRectangleIndices();
	newIndices.erase(std::remove(newIndices.begin(), newIndices.end(), rectIdx), newIndices.end());

	// replace all rectangles previous bounding box
	if (this->fitBoundingBox(newIndices, rectList, oldBox, oldBoxIdx)) {
		newBox->addRectangleIndex(rectIdx);
		oldBox->removeRectangleIndex(rectIdx);

		// delete bounding box in case it is empty
		if (oldBox->getRectangleIndices().size() == 0) {
			bBoxList[oldBoxIdx].reset(); // TODO: bounding box is not destroyed
			bBoxList.erase(bBoxList.begin() + oldBoxIdx);
		}

		return true;
	}

	else {
		return false;
	}
}


/*******
NEIGHBOURS:

METHOD A: 
Place a rectangle at another bounding box and in case it doesn't fit, rotate the rectangle and try to place again

METHOD B:
Select two rectangles and swap them

Current program flow:
With probability of 30 %, try method B first. Try to find a neighbour within 100 iterations. If failed, use method A.
With probability of 70 %, try method A and don't use method B.

IDEA: also use method B after failed method A.
*******/

template<>
inline float GeometryBasedNeighbour<DataHolder*>::optimize() {	

	/* Initialize */
	std::vector<std::shared_ptr<class BoundingBox>> bestBoxList;
	bestData_->getBoxCreator()->getBoundingBoxList(bestBoxList);
	if (bestBoxList.size() == 0) {
		initSol_->CreateInitialSolution(data_);
		bestData_->OverwriteData(data_);
	}

	// get bounding box list
	std::shared_ptr<BoundingBoxCreator> boxCreator = data_->getBoxCreator();
	std::vector<std::shared_ptr<BoundingBox>> bBoxList;
	boxCreator->getBoundingBoxList(bBoxList);

	// get rectangle list
	std::shared_ptr<RectangleCreator> rectCreator = data_->getRectCreator();
	std::vector<class RectangleHolder*>* rectList = rectCreator->getRectList();
	size_t rectListSize = rectList->size();

	if (isFirstIteration_) {
		std::cout << "Start score: " << this->calculateScore(rectListSize, bBoxList) << std::endl;
		isFirstIteration_ = false;
	}

	if (rectListSize <= 1) {
		return this->calculateScore(rectListSize, bBoxList);
	}


	bool foundNeighbour = false;
	int method = rand() % 100;
	int methodB = 30; // use method B, when method value is smaller than MethodB


	if (method < methodB) {

		/******* METHOD B : Swap two rectangles *******/
		std::cout << "method B" << std::endl;


		bool isPlaced1 = false;
		bool isPlaced2 = false;
		size_t iter = 0;

		while (!foundNeighbour && (++iter < 100)) {

			// 1) get two rectangles
			int rectIndex1 = rand() % rectListSize;
			int rectIndex2 = rand() % rectListSize;
			QRectF& rect1 = (*rectList)[rectIndex1]->getRectRef();
			QRectF& rect2 = (*rectList)[rectIndex2]->getRectRef();

			// get box id
			int boxFromRect1 = (*rectList)[rectIndex1]->getBoundingBoxIndex();
			int boxFromRect2 = (*rectList)[rectIndex2]->getBoundingBoxIndex();

			for (int boxIdx = 0; boxIdx < bBoxList.size(); ++boxIdx) {
				std::vector<int> vec = bBoxList[boxIdx]->getRectangleIndices();
				if (std::find(vec.begin(), vec.end(), rectIndex1) != vec.end()) {
					boxFromRect1 = boxIdx;
				}
				if (std::find(vec.begin(), vec.end(), rectIndex2) != vec.end()) {
					boxFromRect2 = boxIdx;
				}
			}
			if (boxFromRect1 == boxFromRect2) {
				continue;
			}

			// TODO / IDEA: keep rectangle order (add index of new rectangle at index position of previous rectangle)

			// Place rect2 and rectangles of boxFromRect1 without rect1 in bounding box from rect1
			bBoxList[boxFromRect1]->removeRectangleIndex(rectIndex1);
			bBoxList[boxFromRect1]->addRectangleIndex(rectIndex2);

			isPlaced1 = this->fitBoundingBox(bBoxList[boxFromRect1]->getRectangleIndices(), rectList, bBoxList[boxFromRect1], boxFromRect1);

			// Place rect1 and rectangles of boxFromRect2 without rect2 in bounding box from rect2
			bBoxList[boxFromRect2]->removeRectangleIndex(rectIndex2);
			bBoxList[boxFromRect2]->addRectangleIndex(rectIndex1);
			isPlaced2 = this->fitBoundingBox(bBoxList[boxFromRect2]->getRectangleIndices(), rectList, bBoxList[boxFromRect2], boxFromRect2);

			if (!isPlaced1 || !isPlaced2) { // revert changes
				data_->OverwriteData(bestData_);
				boxCreator->getBoundingBoxList(bBoxList);
			}

			foundNeighbour = isPlaced1 && isPlaced2;
		}
	}


	/******* METHOD A : Rotate rectangle and place it at another bounding box *******/

	std::cout << "method A" << std::endl;
	
	size_t iteration = 0;

	// iterate through rectangles until a rectangle is found which can be placed in a rotated way
	while (!foundNeighbour && iteration < rectListSize) {

		// 1) select a rectangle
		rectPos = (rectPos >= rectListSize) ? 1 : rectPos + 1;
		int rectIdx = rectListSize - rectPos;
		QRectF& rect = (*rectList)[rectIdx]->getRectRef(); // get a rectangle from list, start with the last one
		int width = rect.width();
		int height = rect.height();
		bool hasRotated = false;

		// 2) get index of rectangle's current bounding box
		int oldBoxIndex = -1;
		// oldBoxIndex = (*rectList)[rectIdx]->getBoundingBoxIndex(); TODO: doesn't work properly!
		for (int boxIdx = 0; boxIdx < bBoxList.size(); ++boxIdx) {
			std::vector<int> vec = bBoxList[boxIdx]->getRectangleIndices();
			if (std::find(vec.begin(), vec.end(), rectIdx) != vec.end()) {
				oldBoxIndex = boxIdx;
			}
		}
		std::shared_ptr<BoundingBox>& oldBox = bBoxList[oldBoxIndex];

		if (oldBoxIndex < 0) {
			std::cout << "**** ERROR: Rectangle in no bounding box ****" << std::endl;
			return AlgorithmConstants::maxScore;
		}
		
		// std::cout << "CHOOSE RECT " << rectIdx << " from box " << (*rectList)[rectIdx]->getBoundingBoxIndex() << std::endl;
		// std::cout << "CHOOSE RECT " << rectIdx << " from box " << oldBoxIndex << std::endl;

		// 3) try to move to another bounding box
		int x, y;
		for (int newBoxIdx = 0; newBoxIdx < bBoxList.size(); ++newBoxIdx) {
			
			if (newBoxIdx == oldBoxIndex) {
				continue; // TODO: handle separatly
			}

			std::shared_ptr<BoundingBox>& newBox = bBoxList[newBoxIdx];

			bool rectFitsInBox = newBox->tryFit((*rectList)[rectIdx], newBoxIdx);
			if (!rectFitsInBox) {
				// rotate rectangle and try to place again
				rect.setWidth(height);
				rect.setHeight(width);
				hasRotated = true;
				rectFitsInBox = newBox->tryFit((*rectList)[rectIdx], newBoxIdx);
			}

			if (rectFitsInBox) {
				foundNeighbour = this->removeRectFromBox(newBox, oldBox, oldBoxIndex, rectIdx, newBoxIdx, bBoxList, rectList);
				if (!foundNeighbour) { // this rectangle can not be replaced since old bounding box cannot be resorted
					data_->OverwriteData(bestData_);
					boxCreator->getBoundingBoxList(bBoxList);
				}
				break;
			}
		}

		// reset rotation since rotated rectangle could not be placed in any bounding box
		if (!foundNeighbour && hasRotated) {
			rect.setWidth(width);
			rect.setHeight(height);
		}

		++iteration;
	}


	 // TODO: durch Tausch kann Score nicht verbessert werden
	if (!foundNeighbour) {
		data_->OverwriteData(bestData_);
		std::cout << "No neighbour could be found" << std::endl;
		return AlgorithmConstants::maxScore;
	}
	else {
		std::cout << "Neighbour found" << std::endl;

		float score = this->calculateScore(rectListSize, bBoxList);

		if (score <= bestScore_) {
			std::cout << "updated" << std::endl;
			bestScore_ = score;
			bestData_->OverwriteData(data_);
		}
		else {
			data_->OverwriteData(bestData_);
		}

		return score;
	}

}
