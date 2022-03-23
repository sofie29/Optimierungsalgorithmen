
#pragma once
#include "stdafx.h"
#include "InitialSolutionI.h"
#include "DataHolder.h"
#include "BoundingBoxCreator.h"
#include "BoundingBox.h"
template<class Data>
class SimpleInitialSolution : public InitialSolutionI<Data> {
public:
	SimpleInitialSolution();
	void CreateInitialSolution(Data d) override;
};

template<class Data>
inline SimpleInitialSolution<Data>::SimpleInitialSolution() : InitialSolutionI<Data>()
{
}

template<class Data>
inline void SimpleInitialSolution<Data>::CreateInitialSolution(Data d)
{
}

template<>
inline void SimpleInitialSolution<DataHolder*>::CreateInitialSolution(DataHolder* d) {
	std::shared_ptr<BoundingBoxCreator> boxCreator = d->getBoxCreator();
	std::shared_ptr<RectangleCreator> rectCreator = d->getRectCreator();

	std::vector<class RectangleHolder*>* rectList = rectCreator->getRectList();
	

	//auto cmp = [](std:: &first, QRectF& second) {return first.width() * first.height() > second.width() * second.height(); };
	//std::sort(rectList.begin(), rectList.end(), cmp);

	

	std::vector<std::shared_ptr<BoundingBox>> bBoxList;
	boxCreator->getBoundingBoxList(bBoxList);

	int amount = rectList->size();
	int recsPerLine = std::min(UIConstants::maxBoxesPerLine, (int)std::ceil(std::sqrt(amount)));
	for (int rectIdx = 0; rectIdx < rectList->size(); ++rectIdx) {
		QRectF& rect = (*rectList)[rectIdx]->getRectRef();
		bool added = false;
		int boxIdx = 0;
		for (std::shared_ptr<BoundingBox> box : bBoxList) {
			//int x, y;
			if (box->tryFit((*rectList)[rectIdx], boxIdx)) {
				box->addRectangleIndex(rectIdx);
				added = true;
				break;
			}
			boxIdx++;
		}
		if (!added) {
			//BoundingBox* box = new BoundingBox();

			int x_pos = (bBoxList.size() % recsPerLine) * (AlgorithmConstants::maxBoxEdgeSize_ + UIConstants::rectangleSpace_);
			int y_pos = (int)(bBoxList.size() / (float)recsPerLine) * (AlgorithmConstants::maxBoxEdgeSize_ + UIConstants::rectangleSpace_);
			boxCreator->addBoundingBox(x_pos, y_pos, (*rectList)[rectIdx], rectIdx, boxIdx);
			boxCreator->getBoundingBoxList(bBoxList);

		}
		//boxCreator->addBoundingBox(rect.x(), rect.y());
	}
	
	rectCreator->setRectList(rectList);
	//boxCreator->OnOptimDone();
}
