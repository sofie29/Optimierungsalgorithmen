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
};

template<class Data>
inline GeometryBasedNeighbour<Data>::GeometryBasedNeighbour(Data data, Data currentBest, InitialSolutionI<Data>* initSol) : NeighbourI<Data>(data, currentBest, initSol) {
	rectPos = 0;
	bestScore_ = 999000;
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
		std::cout << "Start score: " << rectListSize << " " << bBoxList.size() << std::endl;
		std::cout << "Start score: " << this->calculateScore(rectListSize, bBoxList) << std::endl;
		isFirstIteration_ = false;
	}


	bool foundNeighbour = false;


	/******* METHOD A : Rotate rectangle and place it at another bounding box *******/

	// std::cout << "method A" << std::endl;
	
	size_t iteration = 0;

	/*
	for (int boxIdx = 0; boxIdx < bBoxList.size(); ++boxIdx) {
		std::cout << "box " << boxIdx << " has indices ";
		std::vector<int> vec = bBoxList[boxIdx]->getRectangleIndices();
		for (int ind : vec) {
			std::cout << ind << ", ";
		}
		std::cout << "" << std::endl;
	}

	std::cout << "boxes in total: " << bBoxList.size() << std::endl;
	for (int boxIdx = 0; boxIdx < bBoxList.size(); ++boxIdx) {
		std::vector<int> vec = bBoxList[boxIdx]->getRectangleIndices();
		for (int ind : vec) {
			std::cout << "rect: " << ind << ", x: " << (*rectList)[ind]->getRectRef().x() << ", y: " << (*rectList)[ind]->getRectRef().y() << ", width: " << (*rectList)[ind]->getRectRef().width() << ", height: " << (*rectList)[ind]->getRectRef().height() << " ";
			std::cout << "" << std::endl;
		}
		std::cout << "" << std::endl;
	}
	*/
	

	// iterate through rectangles until a rectangle is found which can be placed in a rotated way
	while (!foundNeighbour && iteration < rectListSize) {

		// 1) select a rectangle
		rectPos = (rectPos >= rectListSize) ? 1 : rectPos + 1;
		int rectIdx = rectListSize - rectPos;
		QRectF& rect = (*rectList)[rectIdx]->getRectRef(); // get a rectangle from list, start with the last one
		
		int oldBoxIndex = -1;
		// oldBoxIndex = (*rectList)[rectIdx]->getBoundingBoxIndex();
		// get index in another way:
		for (int boxIdx = 0; boxIdx < bBoxList.size(); ++boxIdx) {
			std::vector<int> vec = bBoxList[boxIdx]->getRectangleIndices();
			if (std::find(vec.begin(), vec.end(), rectIdx) != vec.end()) {
				oldBoxIndex = boxIdx;
			}
		}

		if (oldBoxIndex < 0) {
			std::cout << "**** ERROR: Rectangle in no bounding box ****" << std::endl;
			return AlgorithmConstants::maxScore;
		}

		std::shared_ptr<BoundingBox>& oldBox = bBoxList[oldBoxIndex];
		
		// std::cout << "CHOOSE RECT " << rectIdx << " from box " << (*rectList)[rectIdx]->getBoundingBoxIndex() << std::endl;
		// std::cout << "CHOOSE RECT " << rectIdx << " from box " << oldBoxIndex << std::endl;

		// 2) rotate the selected rectangle
		int width = rect.width();
		int height = rect.height();
		rect.setWidth(height);
		rect.setHeight(width);

		// 3) try to move to another bounding box
		int x, y;
		for (int newBoxIdx = 0; newBoxIdx < bBoxList.size(); ++newBoxIdx) {

			if (newBoxIdx == oldBoxIndex) {
				continue; // must be handled separatly
			}

			std::shared_ptr<BoundingBox>& newBox = bBoxList[newBoxIdx];
			int x_old_rect = rect.x();
			int y_old_rect = rect.y();

			if (newBox->tryFit((*rectList)[rectIdx], newBoxIdx)) {
				std::vector<int> newIndices = oldBox->getRectangleIndices();
				newIndices.erase(std::remove(newIndices.begin(), newIndices.end(), rectIdx), newIndices.end());

				// 4) replace all rectangles previous bounding box
				if (this->fitBoundingBox(newIndices, rectList, oldBox, newBoxIdx)) {
					newBox->addRectangleIndex(rectIdx);
					oldBox->removeRectangleIndex(rectIdx);
					foundNeighbour = true;
				}

				else { // revert changes
					std::cout << "************ STOP **********" << std::endl;
					data_->OverwriteData(bestData_);
					return AlgorithmConstants::maxScore;
				}
			}
		}

		// rotated rectangle could not be placed in any bounding box
		if (!foundNeighbour) {
			rect.setWidth(width);
			rect.setHeight(height);
		}

		++iteration;
	}

	float score = this->calculateScore(rectListSize, bBoxList);
	if (score < bestScore_) {
		bestScore_ = score;
		bestData_->OverwriteData(data_);
	}
	else {
		data_->OverwriteData(bestData_);
	}
	
	return score;






	if (!foundNeighbour && rectListSize > 1 && false) { // search for another neighbour

		/******* METHOD B : Swap two rectangles *******/

		std::cout << "method B" << std::endl;

		bool isPlaced1 = false;
		bool isPlaced2 = false;
		size_t iter = 0;

		while ((!isPlaced1 || !isPlaced2) && (++iter < 2)) {
			std::cout << "try method b" << iter << std::endl;


			// 1) get two rectangles
			int rectIndex1 = 0; // TODO: random
			int rectIndex2 = 1;
			QRectF& rect1 = (*rectList)[rectIndex1]->getRectRef();
			QRectF& rect2 = (*rectList)[rectIndex2]->getRectRef();

			int boxFromRect1 = (*rectList)[rectIndex1]->getBoundingBoxIndex(); 
			int boxFromRect2 = (*rectList)[rectIndex2]->getBoundingBoxIndex();


			// 2) get lists of rectangles of their bounding boxes

			// tryFit with rectangles of boxFromRect1 without rect 1 and with rect2
			bBoxList[boxFromRect1]->removeRectangleIndex(rectIndex1);
			bBoxList[boxFromRect1]->addRectangleIndex(rectIndex2);
			isPlaced1 = this->fitBoundingBox(bBoxList[boxFromRect1]->getRectangleIndices(), rectList, bBoxList[boxFromRect1], boxFromRect1);

			// tryFit with rectangles of boxFromRect2 without rect 2 and with rect1
			bBoxList[boxFromRect2]->removeRectangleIndex(rectIndex2);
			bBoxList[boxFromRect2]->addRectangleIndex(rectIndex1);
			isPlaced2 = this->fitBoundingBox(bBoxList[boxFromRect2]->getRectangleIndices(), rectList, bBoxList[boxFromRect2], boxFromRect2);

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
		data_->OverwriteData(bestData_);
		std::cout << "No neighbour could be found" << std::endl;
		return AlgorithmConstants::maxScore; // return max score value
	}

	bestData_->OverwriteData(data_);

	return this->calculateScore(rectListSize, bBoxList);;
}
