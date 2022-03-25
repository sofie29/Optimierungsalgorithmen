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
	
	


	auto t1 = std::chrono::high_resolution_clock::now();


	std::shared_ptr<BoundingBoxCreator> boxCreator = data_->getData()->getBoxCreator();
	std::shared_ptr<RectangleCreator> rectCreator = data_->getData()->getRectCreator();

	std::vector<class RectangleHolder*>* rectList =  rectCreator->getRectList();
	
	auto rd = std::random_device{};
	auto rng = std::default_random_engine{ rd() };
	std::shuffle(std::begin(*rectList), std::end(*rectList), rng);


	std::vector<std::shared_ptr<BoundingBox>> bBoxList;
	boxCreator->getBoundingBoxList(bBoxList);

	int amount = rectList->size();
	int recsPerLine = std::min(UIConstants::maxBoxesPerLine, (int) std::ceil(std::sqrt(amount)));
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
		
	}
	
	
	
	rectCreator->setRectList(rectList);

	auto t2 = std::chrono::high_resolution_clock::now();
	auto ms_int1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
	std::cout << ms_int1.count() << std::endl;
	

	
	return bBoxList.size();
}