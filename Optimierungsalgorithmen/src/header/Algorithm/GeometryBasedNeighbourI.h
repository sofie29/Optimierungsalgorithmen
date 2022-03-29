#pragma once
#include <BoundingBox.h>
#include "NeighbourI.h"
#include "DataHolder.h"
#include "BoundingBoxCreator.h"
#include "RectangleCreator.h"
#include "InitialSolutionI.h"
#include "BoundingBox.h"
#include "RectangleHolder.h"


template<class Data>
class GeometryBasedNeighbourI : public NeighbourI<Data>
{
public:
	GeometryBasedNeighbourI(DataHolderT<Data>* data, DataHolderT<Data>* currentBest, InitialSolutionI<Data>* initSol);

	virtual float optimize() = 0;
	virtual void resetData() override;

	virtual void initParameters() = 0; // TODO: bestScore_ = AlgorithmConstants::maxScore;
	virtual void shiftScore(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) = 0;
	virtual float calculateScore(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) = 0;
	virtual bool tryFitWrapper(std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIdx, std::vector<class RectangleHolder*>* rectangles, int rectIdx, bool multipleRects) = 0;
	virtual void handleEmptyBoundingBox(std::shared_ptr<BoundingBoxCreator> boxCreator, std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIndex) = 0;
	virtual int getRectPosMethodA(int rectListSize) = 0;

protected:
	float bestScore_;
	bool fitBoundingBox(std::vector<int> indices, std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIndex);
	bool removeRectFromBox(std::shared_ptr<BoundingBox>& newBox, std::shared_ptr<BoundingBox>& oldBox, int oldBoxIdx, int rectIdx, int newBoxIdx, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, std::vector<class RectangleHolder*>* rectList);
	bool resetData_;
	float findNeighbour(bool withoutOverlapping);
	void setAllToDefaultColors(std::vector<class RectangleHolder*>* rectList);
	void resetBestScore();
};


template<class Data>
inline GeometryBasedNeighbourI<Data>::GeometryBasedNeighbourI(DataHolderT<Data>* data, DataHolderT<Data>* currentBest, InitialSolutionI<Data>* initSol) : NeighbourI<Data>(data, currentBest, initSol) {
	bestScore_ = AlgorithmConstants::maxScore;
	resetData_ = false;
}


template<class Data>
inline void GeometryBasedNeighbourI<Data>::resetData()
{
	if (resetData_) {
		resetData_ = false;
		NeighbourI<Data>::data_->OverwriteData(NeighbourI<Data>::bestData_);
	}
}



// attention: this method doesn't update index vector of the bounding box
template<>
inline bool GeometryBasedNeighbourI<DataHolder*>::fitBoundingBox(std::vector<int> indices, std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIndex) {
	boxList[boxIndex]->removeLowerLevelBoundingBoxes();

	// place all rectangles specified in indices at the given bounding box
	for (int i : indices) {
		//if (!box->tryFit((*rectangles)[i], boundingBoxIndex)) {
		if (!this->tryFitWrapper(boxList, boxIndex, rectangles, i, true)) {
			return false;
		}
	}
	return true;
}

template<>
inline bool GeometryBasedNeighbourI<DataHolder*>::removeRectFromBox(std::shared_ptr<BoundingBox>& newBox, std::shared_ptr<BoundingBox>& oldBox, int oldBoxIdx, int rectIdx, int newBoxIdx, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, std::vector<class RectangleHolder*>* rectList) {
	std::vector<int> newIndices = oldBox->getRectangleIndices();
	newIndices.erase(std::remove(newIndices.begin(), newIndices.end(), rectIdx), newIndices.end());

	// replace all rectangles previous bounding box
	if (this->fitBoundingBox(newIndices, rectList, bBoxList, oldBoxIdx)) {
		newBox->addRectangleIndex(rectIdx);
		oldBox->removeRectangleIndex(rectIdx);

		// delete bounding box in case it is empty
		if (oldBox->getRectangleIndices().size() == 0) {
			this->handleEmptyBoundingBox(data_->getData()->getBoxCreator(), bBoxList, oldBoxIdx);
		}

		return true;
	}

	else {
		// std::cout << "cannot fit old box  " << std::endl;
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
inline float GeometryBasedNeighbourI<DataHolder*>::findNeighbour(bool withoutOverlapping) {
	float score;

	// get bounding box list
	std::shared_ptr<BoundingBoxCreator> boxCreator = data_->getData()->getBoxCreator();
	std::vector<std::shared_ptr<BoundingBox>> bBoxList;
	boxCreator->getBoundingBoxList(bBoxList);

	// get rectangle list
	std::shared_ptr<RectangleCreator> rectCreator = data_->getData()->getRectCreator();
	std::vector<class RectangleHolder*>* rectList = rectCreator->getRectList();
	size_t rectListSize = rectList->size();

	if (rectListSize <= 1) {
		return this->calculateScore(rectList, bBoxList);
	}


	bool foundNeighbour = false;
	int method = rand() % 100;
	int probabilityB = 10; // use method B, when method value is smaller than MethodB


	if (withoutOverlapping && method < probabilityB) {

		/******* METHOD B : Swap two rectangles *******/
		
		// std::cout << "method B" << std::endl;


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

			bool sameBox = boxFromRect1 == boxFromRect2;
			bool oneElementBox = bBoxList[boxFromRect1]->getRectangleIndices().size() == 1 && bBoxList[boxFromRect2]->getRectangleIndices().size() == 1;
			if (sameBox || oneElementBox) {
				continue;
			}

			// TODO / IDEA: keep rectangle order (add index of new rectangle at index position of previous rectangle)

			// Place rect2 and rectangles of boxFromRect1 without rect1 in bounding box from rect1
			bBoxList[boxFromRect1]->removeRectangleIndex(rectIndex1);
			bBoxList[boxFromRect1]->addRectangleIndex(rectIndex2);

			isPlaced1 = this->fitBoundingBox(bBoxList[boxFromRect1]->getRectangleIndices(), rectList, bBoxList, boxFromRect1);

			// Place rect1 and rectangles of boxFromRect2 without rect2 in bounding box from rect2
			bBoxList[boxFromRect2]->removeRectangleIndex(rectIndex2);
			bBoxList[boxFromRect2]->addRectangleIndex(rectIndex1);
			isPlaced2 = this->fitBoundingBox(bBoxList[boxFromRect2]->getRectangleIndices(), rectList, bBoxList, boxFromRect2);

			if (!isPlaced1 || !isPlaced2) { // revert changes
				data_->getData()->OverwriteData(bestData_->getData());
				boxCreator->getBoundingBoxList(bBoxList);
			}

			foundNeighbour = isPlaced1 && isPlaced2;

			if (foundNeighbour) {
				this->setAllToDefaultColors(rectList);
				(*rectList)[rectIndex1]->setToSwappedColor();
				(*rectList)[rectIndex2]->setToSwappedColor();
				score = this->calculateScore(rectList, bBoxList) - 0.01;
			}
		}
	}


	/******* METHOD A : Rotate rectangle and place it at another bounding box *******/

	// std::cout << "method A" << std::endl;

	size_t iteration = 0;

	// iterate through rectangles until a rectangle is found which can be placed in a rotated way
	while (!foundNeighbour && iteration < rectListSize) {

		// 1) select a rectangle
		int rectIdx = this->getRectPosMethodA(rectListSize);
		// std::cout << rectIdx << std::endl;
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
			if (!withoutOverlapping) newBoxIdx = rand() % bBoxList.size();
			// std::cout << "try box  " << newBoxIdx << std::endl;


			if (newBoxIdx == oldBoxIndex) {
				continue; // must be handled separatly
			}

			std::shared_ptr<BoundingBox>& newBox = bBoxList[newBoxIdx];

			bool rectFitsInBox = this->tryFitWrapper(bBoxList, newBoxIdx, rectList, rectIdx, false);
			if (!rectFitsInBox) {
				// rotate rectangle and try to place again
				rect.setWidth(height);
				rect.setHeight(width);
				hasRotated = true;
				rectFitsInBox = this->tryFitWrapper(bBoxList, newBoxIdx, rectList, rectIdx, false);
			}

			if (rectFitsInBox) {
				foundNeighbour = this->removeRectFromBox(newBox, oldBox, oldBoxIndex, rectIdx, newBoxIdx, bBoxList, rectList);
				if (!foundNeighbour) { // this rectangle can not be replaced since old bounding box cannot be resorted
					data_->getData()->OverwriteData(bestData_->getData());
					boxCreator->getBoundingBoxList(bBoxList);
				}
				else {
					this->setAllToDefaultColors(rectList);
					(*rectList)[rectIdx]->setToSwappedColor();
					score = this->calculateScore(rectList, bBoxList);
				}
				break;

			}
			else {
				// reset rotation
				if (hasRotated) {
					hasRotated = false;
					rect.setWidth(width);
					rect.setHeight(height);
				}
			}
		}
		if (!foundNeighbour && !withoutOverlapping) {
			// add bounding box (nur wenn von Transgression list?)
			// std::cout << "boxlist size: " << bBoxList.size() << std::endl;
			int amount = rectList->size();
			int recsPerLine = std::min(UIConstants::maxBoxesPerLine, (int)std::ceil(std::sqrt(amount)));
			int x_pos = (bBoxList.size() % recsPerLine) * (AlgorithmConstants::maxBoxEdgeSize_ + UIConstants::rectangleSpace_);
			int y_pos = (int)(bBoxList.size() / (float)recsPerLine) * (AlgorithmConstants::maxBoxEdgeSize_ + UIConstants::rectangleSpace_);

			boxCreator->addBoundingBox(x_pos, y_pos, (*rectList)[rectIdx], rectIdx, bBoxList.size());
			boxCreator->getBoundingBoxList(bBoxList);
			// std::cout << "added bounding box, boxlist size: " << bBoxList.size() << std::endl;
			// Algorithmus muss sie auch nutzen
			foundNeighbour = true;
			break;
		}

		++iteration;
	}


	if (!foundNeighbour) {
		std::cout << "no neighbour" << std::endl;
		score = AlgorithmConstants::maxScore;
	}

	else {
		score = this->calculateScore(rectList, bBoxList);
	}

	resetData_ = score >= bestScore_;
	// std::cout << "score: " << score << " bestscore: " << bestScore_ << std::endl;
	if (score <= bestScore_) {
		this->shiftScore(rectList, bBoxList);
		bestScore_ = score;
	}

	return score;
}

template<class Data>
inline void GeometryBasedNeighbourI<Data>::setAllToDefaultColors(std::vector<class RectangleHolder*>* rectList)
{
	for (RectangleHolder* rect : *rectList) {
		rect->setToDefaultColor();
	}
}

template<class Data>
inline void GeometryBasedNeighbourI<Data>::resetBestScore()
{
	bestScore_ = AlgorithmConstants::maxScore;
}


