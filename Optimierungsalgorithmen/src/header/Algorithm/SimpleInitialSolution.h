
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

	std::vector<QRectF> rectList;
	rectCreator->getRectList(rectList);

	auto cmp = [](QRectF& first, QRectF& second) {return first.width() * first.height() > second.width() * second.height(); };
	std::sort(rectList.begin(), rectList.end(), cmp);

	

	std::vector<std::shared_ptr<BoundingBox>> bBoxList;
	boxCreator->getBoundingBoxList(bBoxList);

	int amount = rectList.size();
	int recsPerLine = std::min(UIConstants::maxBoxesPerLine, (int)std::ceil(std::sqrt(amount)));
	for (int i = 0; i < rectList.size(); ++i) {
		QRectF& rect = rectList[i];
		bool added = false;
		for (std::shared_ptr<BoundingBox> box : bBoxList) {
			int x, y;
			if (box->tryFit(rect.width(), rect.height(), x, y)) {
				box->addRectangleIndex(i);
				added = true;
				rect.moveTopLeft(QPointF(x, y));
				break;
			}

		}
		if (!added) {
			//BoundingBox* box = new BoundingBox();
			
			int x_pos = (bBoxList.size() % recsPerLine) * (AlgorithmConstants::maxBoxEdgeSize_ + UIConstants::rectangleSpace_);
			int y_pos = (int)(bBoxList.size() / (float)recsPerLine) * (AlgorithmConstants::maxBoxEdgeSize_ + UIConstants::rectangleSpace_);
			boxCreator->addBoundingBox(x_pos, y_pos, rect, i);
			boxCreator->getBoundingBoxList(bBoxList);
		}
		//boxCreator->addBoundingBox(rect.x(), rect.y());
	}
	
	rectCreator->setRectList(rectList);
	//boxCreator->OnOptimDone();
}
