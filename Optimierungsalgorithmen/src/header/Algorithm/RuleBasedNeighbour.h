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
	virtual void resetData() override;
	
};


template<class Data>
inline RuleBasedNeighbour<Data>::RuleBasedNeighbour(DataHolderT<Data>* data, DataHolderT<Data>* currentBest, InitialSolutionI<Data>* initSol) : NeighbourI<Data>(data, currentBest, initSol)
{
	//NeighbourI<Data>::data_ = data;
}


template<class Data>
inline float RuleBasedNeighbour<Data>::optimize()
{
}

template<class Data>
inline void RuleBasedNeighbour<Data>::resetData()
{
	NeighbourI<Data>::data_->ResetData();
}

template<>
inline float RuleBasedNeighbour<DataHolder*>::optimize() {
	


	


	std::shared_ptr<BoundingBoxCreator> boxCreator = data_->getData()->getBoxCreator();
	std::shared_ptr<RectangleCreator> rectCreator = data_->getData()->getRectCreator();

	std::vector<class RectangleHolder*>* rectList =  rectCreator->getRectList();
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
	int recsPerLine = std::min(UIConstants::maxBoxesPerLine, (int) std::ceil(std::sqrt(amount)));
	int idx = 0;
	for (class RectangleHolder* rect : *rectList) {
		bool added = false;
		int idx = 0;
		for (std::shared_ptr<BoundingBox> box : bBoxList) {
			//int x, y;
			if (box->tryFit(rect, idx)) {
				added = true;
				break;
			}
			idx++;
		}
		if (!added) {
			int x_pos = (bBoxList.size() % recsPerLine) * (AlgorithmConstants::maxBoxEdgeSize_ + UIConstants::rectangleSpace_);
			int y_pos = (int)(bBoxList.size() / (float)recsPerLine) * (AlgorithmConstants::maxBoxEdgeSize_ + UIConstants::rectangleSpace_);
			boxCreator->addBoundingBox(x_pos, y_pos, rect->getRectRef());
			boxCreator->getBoundingBoxList(bBoxList);
		}
		
		rect->setToDefaultColor();
	}
	(*rectList)[firstIndex]->setToSwappedColor();
	(*rectList)[secondIndex]->setToSwappedColor();
	
	
	rectCreator->setRectList(rectList);

	
	

	
	return bBoxList.size();
}