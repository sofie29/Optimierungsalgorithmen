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
	virtual void afterOptimization() override;
	virtual void resetData() override;

	virtual void initParameters() = 0;
	virtual float calculateScore(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) = 0;
	virtual bool tryFitWrapper(std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIdx, std::vector<class RectangleHolder*>* rectangles, int rectIdx, bool multipleRects) = 0;
	virtual int getRectPos(const int rectListSize, std::vector<std::shared_ptr<BoundingBox>>& boxList, bool& isTransgressionRect) = 0;
	virtual int getBoxPos(const int boxListSize, std::vector<std::shared_ptr<BoundingBox>>& boxList) = 0;
	virtual void resetBoxPos() = 0;
	
	// for overlapping
	virtual void shiftScore(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) = 0;
	virtual int calculateOverlappingWrapper(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) = 0;
	virtual void updateBoxQueue(int boxIndex1, int boxIndex2, std::vector<std::shared_ptr<BoundingBox>>& boxList, std::vector<class RectangleHolder*>* rectangles, bool deleteBox1) = 0;

protected:
	float bestScore_;
	int initalNumberOfBoxes_;
	bool fitBoundingBox(std::vector<int> indices, std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIndex);
	bool removeRectFromBox(std::shared_ptr<BoundingBox>& oldBox, int oldBoxIdx, int rectIdx, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, std::vector<class RectangleHolder*>* rectList, std::shared_ptr<BoundingBox>* newBox = NULL);
	void handleEmptyBoundingBox(std::shared_ptr<BoundingBoxCreator> boxCreator, std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIndex);
	bool resetData_;
	float findNeighbour(bool withoutOverlapping);
	void setAllToDefaultColors(std::vector<class RectangleHolder*>* rectList);

	bool overlapping_;
	int numberOfAddedBoxes_;
};


template<class Data>
inline GeometryBasedNeighbourI<Data>::GeometryBasedNeighbourI(DataHolderT<Data>* data, DataHolderT<Data>* currentBest, InitialSolutionI<Data>* initSol) : NeighbourI<Data>(data, currentBest, initSol) {
	bestScore_ = AlgorithmConstants::maxScore;
	resetData_ = false;
	overlapping_ = false;
	numberOfAddedBoxes_ = 0;
}


template<>
inline void GeometryBasedNeighbourI<DataHolder*>::afterOptimization()
{
	// get bounding box list
	std::vector<std::shared_ptr<BoundingBox>> bBoxList;
	data_->getData()->getBoxCreator()->getBoundingBoxList(bBoxList);

	// get rectangle list
	std::vector<class RectangleHolder*>* rectList = data_->getData()->getRectCreator()->getRectList();

	int transgressions = this->calculateOverlappingWrapper(rectList, bBoxList);
	// if (transgressions > 0) std::cout << "Number of remaining transgressions: " << transgressions << std::endl;
	std::cout << "Number of remaining transgressions: " << transgressions << std::endl;
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
	// TODO: speichern der alten Box, so versuchen, und falls es nicht klappt keine aenderung ander transgression wegmachen

	// place all rectangles specified in indices at the given bounding box
	for (int i : indices) {
		if (!this->tryFitWrapper(boxList, boxIndex, rectangles, i, true)) {
			return false;
		}
	}
	return true;
}

template<>
inline bool GeometryBasedNeighbourI<DataHolder*>::removeRectFromBox(std::shared_ptr<BoundingBox>& oldBox, int oldBoxIdx, int rectIdx, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, std::vector<class RectangleHolder*>* rectList, std::shared_ptr<BoundingBox>* newBox) {
	//std::vector<int> newIndices = oldBox->getRectangleIndices();
	// newIndices.erase(std::remove(newIndices.begin(), newIndices.end(), rectIdx), newIndices.end());
	oldBox->removeRectangleIndex(rectIdx); // TODO: delete before if, and add in case if fails

	// replace all rectangles previous bounding box
	if (this->fitBoundingBox(oldBox->getRectangleIndices(), rectList, bBoxList, oldBoxIdx)) {
		if (newBox != nullptr) (*newBox)->addRectangleIndex(rectIdx);

		// delete bounding box in case it is empty
		if (oldBox->getRectangleIndices().size() == 0) {
			this->handleEmptyBoundingBox(data_->getData()->getBoxCreator(), bBoxList, oldBoxIdx);
		}

		return true;
	}

	else {
		oldBox->addRectangleIndex(rectIdx);
		return false;
	}
}

template<class Data>
inline void GeometryBasedNeighbourI<Data>::handleEmptyBoundingBox(std::shared_ptr<BoundingBoxCreator> boxCreator, std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIndex)
{
	boxCreator->resetOneBoundingBox(boxIndex);
	boxCreator->getBoundingBoxList(boxList);
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
	int changedBox1, changedBox2, changedBox1newIndex;
	bool deleteBox1 = false;
	overlapping_ = !withoutOverlapping;

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


	if (!overlapping_ && method < probabilityB) {

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
				changedBox1 = boxFromRect1;
				changedBox2 = boxFromRect2;
			}
		}
	}


	/******* METHOD A : Rotate rectangle and place it at another bounding box *******/

	// std::cout << "method A" << std::endl;

	size_t iteration = 0;

	// iterate through rectangles until a rectangle is found which can be placed in a rotated way
	while (!foundNeighbour && iteration < rectListSize) {

		// 1) select a rectangle
		bool isTransgressionRect = false;
		int rectIdx = this->getRectPos(rectListSize, bBoxList, isTransgressionRect);
		RectangleHolder* rectHolder = (*rectList)[rectIdx];
		QRectF& rect = rectHolder->getRectRef();

		// skip rectangle with a probability of 70 % in case its area is greater than 75 % of the bounding box
		while (!isTransgressionRect && rect.width() * rect.height() > 0.75 * bBoxList[0]->getBoxWidth() * bBoxList[0]->getBoxWidth()) {
			if (rand() % 100 < 30) break;
			// std::cout << "skipped rectangle with idx " << rectIdx << std::endl;
			rectIdx = this->getRectPos(rectListSize, bBoxList, isTransgressionRect);
			RectangleHolder* rectHolder = (*rectList)[rectIdx];
			QRectF& rect = rectHolder->getRectRef(); // get a rectangle from list, start with the last one
		}
		
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
		std::cout << "CHOOSE RECT " << rectIdx << " from box " << oldBoxIndex << std::endl;

		// 3) try to move to another bounding box
		int boxIteration = 0;
		int newBoxIdx = -1;
		this->resetBoxPos();
		while (boxIteration++ < bBoxList.size()) {
			newBoxIdx = this->getBoxPos(bBoxList.size(), bBoxList);

			// std::cout << "try box  " << newBoxIdx << std::endl;

			if (newBoxIdx == oldBoxIndex) {
				continue; // must be handled separatly
			}

			std::shared_ptr<BoundingBox>& newBox = bBoxList[newBoxIdx];

			bool rectFitsInBox = this->tryFitWrapper(bBoxList, newBoxIdx, rectList, rectIdx, false);
			if (!rectFitsInBox) {
				// rotate rectangle and try to place again
				rectHolder->rotateRect();
				hasRotated = true;
				rectFitsInBox = this->tryFitWrapper(bBoxList, newBoxIdx, rectList, rectIdx, false);
			}
			
			if (rectFitsInBox) {
				int oldBoxListSize = bBoxList.size();
				// std::cout << "rect fits in box " << rectFitsInBox << std::endl;
				foundNeighbour = this->removeRectFromBox(oldBox, oldBoxIndex, rectIdx, bBoxList, rectList, &newBox);
				if (foundNeighbour) {
					this->setAllToDefaultColors(rectList);
					(*rectList)[rectIdx]->setToSwappedColor();
					score = this->calculateScore(rectList, bBoxList);
					changedBox1 = oldBoxIndex;
					changedBox2 = newBoxIdx;
					deleteBox1 = bBoxList.size() < oldBoxListSize;
				}
				else { // this rectangle can not be replaced since old bounding box cannot be resorted
					data_->getData()->OverwriteData(bestData_->getData());
					boxCreator->getBoundingBoxList(bBoxList);
					// std::cout << "old box cannot be reordered" << std::endl;
				}
				break;

			}
			else {
				// reset rotation
				if (hasRotated) {
					hasRotated = false;
					rectHolder->rotateRect();
				}
			}
		}

		// add bounding box
		/*
		std::cout << "could add bounding box" << std::endl;
		std::cout << oldBox->getRectangleIndices().size() << std::endl;
		std::cout << foundNeighbour << std::endl;
		std::cout << overlapping_ << std::endl;
		*/

		if (!foundNeighbour && overlapping_ && isTransgressionRect && rand() % 100 < 50 && oldBox->getRectangleIndices().size() > 1) {
			// TODO: modify box position !!!!!!!
			if (this->removeRectFromBox(oldBox, oldBoxIndex, rectIdx, bBoxList, rectList)) {
				int amount = rectListSize + numberOfAddedBoxes_++;
				int recsPerLine = std::min(UIConstants::maxBoxesPerLine, (int)std::ceil(std::sqrt(amount)));
				int x_pos = (amount % recsPerLine) * (AlgorithmConstants::maxBoxEdgeSize_ + UIConstants::rectangleSpace_);
				int y_pos = (int)(amount / (float)recsPerLine) * (AlgorithmConstants::maxBoxEdgeSize_ + UIConstants::rectangleSpace_);

				std::cout << "position" << x_pos << ", " << y_pos << std::endl;
				boxCreator->addBoundingBox(x_pos, y_pos, (*rectList)[rectIdx], rectIdx, bBoxList.size());
				boxCreator->getBoundingBoxList(bBoxList);
				foundNeighbour = true;
				changedBox1 = oldBoxIndex;
				changedBox2 = int(bBoxList.size()) - 1;
				score = this->calculateScore(rectList, bBoxList);
				break;
			 }
			else { // this rectangle can not be replaced since old bounding box cannot be resorted
				std::cout << "old Box cannot be reordered" << std::endl;

				data_->getData()->OverwriteData(bestData_->getData());
				boxCreator->getBoundingBoxList(bBoxList);
			}
		}

		++iteration;
	}

	if (!foundNeighbour) {
		std::cout << "no neighbour" << std::endl;
		score = AlgorithmConstants::maxScore;
	}

	resetData_ = score >= bestScore_;
	// std::cout << "score: " << score << " bestscore: " << bestScore_ << std::endl;
	if (score <= bestScore_) {
		this->shiftScore(rectList, bBoxList);
	}

	if (score < bestScore_) {
		bestScore_ = score;
		this->updateBoxQueue(changedBox1, changedBox2, bBoxList, rectList, deleteBox1);
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
