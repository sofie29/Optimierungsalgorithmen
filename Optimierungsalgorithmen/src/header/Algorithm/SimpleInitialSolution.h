
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
	void CreateInitialSolution(DataHolderT<Data>* d, bool limitInitRectsPerBox) override;
};

template<class Data>
inline SimpleInitialSolution<Data>::SimpleInitialSolution() : InitialSolutionI<Data>()
{
}

template<class Data>
inline void SimpleInitialSolution<Data>::CreateInitialSolution(DataHolderT<Data>* d, bool limitInitRectsPerBox)
{
}

template<>
inline void SimpleInitialSolution<DataHolder*>::CreateInitialSolution(DataHolderT<DataHolder*>* d, bool limitInitRectsPerBox) {
	std::shared_ptr<BoundingBoxCreator> boxCreator = d->getData()->getBoxCreator();
	std::shared_ptr<RectangleCreator> rectCreator = d->getData()->getRectCreator();

	std::vector<class RectangleHolder*>* rectList = rectCreator->getRectList();

	std::vector<std::shared_ptr<BoundingBox>> bBoxList;
	boxCreator->getBoundingBoxList(bBoxList);

	int amount = rectList->size();
	int recsPerLine = UIConstants::maxBoxesPerLine;
	for (int rectIdx = 0; rectIdx < rectList->size(); ++rectIdx) {
		QRectF& rect = (*rectList)[rectIdx]->getRectRef();
		bool added = false;
		int boxIdx = 0;

		for (std::shared_ptr<BoundingBox> box : bBoxList) {
			//int x, y;
			if ((!limitInitRectsPerBox || box->getRectangleIndices().size() < AlgorithmConstants::maxInitialRectsPerBox_) && box->tryFit((*rectList)[rectIdx], boxIdx)) {
				box->addRectangleIndex(rectIdx);
				added = true;
				break;
			}
			boxIdx++;
		}

		if (!added) {
			int x_pos = (bBoxList.size() % recsPerLine) * (AlgorithmConstants::maxBoxEdgeSize_ + UIConstants::rectangleSpace_);
			int y_pos = (int)(bBoxList.size() / (float)recsPerLine) * (AlgorithmConstants::maxBoxEdgeSize_ + UIConstants::rectangleSpace_);
			boxCreator->addBoundingBox(x_pos, y_pos, (*rectList)[rectIdx], rectIdx, boxIdx);
			boxCreator->getBoundingBoxList(bBoxList);

		}

	}
	boxCreator->RemoveEmptyBoundingBoxes();

}
