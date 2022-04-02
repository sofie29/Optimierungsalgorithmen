#pragma once
#include "NeighbourI.h"
#include "DataHolder.h"
#include "BoundingBoxCreator.h"
#include "RectangleCreator.h"
#include "InitialSolutionI.h"
#include "BoundingBox.h"
#include "RectangleHolder.h"




template<class Data>
class RuleBasedNeighbour : public NeighbourI<Data> {
public:
	RuleBasedNeighbour(DataHolderT<Data>* data, DataHolderT<Data>* currentBest,  InitialSolutionI<Data>* initSol);
	
	virtual float optimize() override;
	virtual void postOptimStep(float newScore, float oldScore) override;
	virtual void resetData() override;
	virtual void initParameters() override;
	virtual void afterOptimization() override;
	virtual float getScoreInformation() override;
	
};


template<class Data>
inline RuleBasedNeighbour<Data>::RuleBasedNeighbour(DataHolderT<Data>* data, DataHolderT<Data>* currentBest, InitialSolutionI<Data>* initSol) : NeighbourI<Data>(data, currentBest, initSol)
{
	//NeighbourI<Data>::data_ = data;
	NeighbourI<Data>::identifier_ = "RuleBasedNeighbour";
}


template<class Data>
inline float RuleBasedNeighbour<Data>::optimize()
{
}

template<class Data>
inline void RuleBasedNeighbour<Data>::postOptimStep(float newScore, float oldScore)
{
	this->resetData();
}

template<class Data>
inline void RuleBasedNeighbour<Data>::resetData()
{
	NeighbourI<Data>::data_->SoftResetData();
}

template<class Data>
inline void RuleBasedNeighbour<Data>::initParameters()
{
}

template<class Data>
inline void RuleBasedNeighbour<Data>::afterOptimization()
{
}

template<class Data>
inline float RuleBasedNeighbour<Data>::getScoreInformation()
{
	return 0.0f;
}

template<>
inline void RuleBasedNeighbour<DataHolder*>::afterOptimization()
{
	std::shared_ptr<BoundingBoxCreator> boxCreator = NeighbourI<DataHolder*>::bestData_->getData()->getBoxCreator();
	boxCreator->RemoveEmptyBoundingBoxes();


}

template<>
inline float RuleBasedNeighbour<DataHolder*>::optimize() {






	std::shared_ptr<BoundingBoxCreator> boxCreator = data_->getData()->getBoxCreator();
	std::shared_ptr<RectangleCreator> rectCreator = data_->getData()->getRectCreator();

	std::vector<class RectangleHolder*>* rectList = rectCreator->getRectList();
	if (rectList->size() == 1) return 1;

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, rectList->size() - 1); // distribution in range [x, y]
	int firstIndex = dist(rng);
	int secondIndex = firstIndex;
	while (secondIndex == firstIndex) {
		secondIndex = dist(rng);
	}
	RectangleHolder* tmp = (*rectList)[firstIndex];
	(*rectList)[firstIndex] = (*rectList)[secondIndex];
	(*rectList)[secondIndex] = tmp;


	std::vector<std::shared_ptr<BoundingBox>> bBoxList;
	boxCreator->getBoundingBoxList(bBoxList);

	int amount = rectList->size();
	int recsPerLine = std::min(UIConstants::maxBoxesPerLine, (int)std::ceil(std::sqrt(amount)));
	int rectIdx = 0;
	for (class RectangleHolder* rect : *rectList) {
		bool added = false;
		int boxIdx = 0;
		for (std::shared_ptr<BoundingBox> box : bBoxList) {
			//int x, y;
			if (box->tryFit(rect, boxIdx)) {
				box->addRectangleIndex(rectIdx);
				added = true;
				break;
			}
			boxIdx++;
		}
		if (!added) {
			int x_pos = (bBoxList.size() % recsPerLine) * (AlgorithmConstants::maxBoxEdgeSize_ + UIConstants::rectangleSpace_);
			int y_pos = (int)(bBoxList.size() / (float)recsPerLine) * (AlgorithmConstants::maxBoxEdgeSize_ + UIConstants::rectangleSpace_);
			boxCreator->addBoundingBox(x_pos, y_pos, rect, rectIdx, boxIdx);
			boxCreator->getBoundingBoxList(bBoxList);
		}

		rect->setToDefaultColor();
		rectIdx++;
	}
	(*rectList)[firstIndex]->setToSwappedColor();
	(*rectList)[secondIndex]->setToSwappedColor();


	rectCreator->setRectList(rectList);





	return 1.0f - boxCreator->getCurrentFitScore();
}