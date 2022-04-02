#pragma once
#include "stdafx.h"
#include "SortingStrategyI.h"
#include "DataHolder.h"
#include "RectangleCreator.h"
#include "RectangleHolder.h"
template<class Data>
class DiagonalSortingStrategy : public SortingStrategyI<Data> {
public:
	DiagonalSortingStrategy();
	virtual void sort(DataHolderT<Data>* data) override;
};

template<class Data>
inline DiagonalSortingStrategy<Data>::DiagonalSortingStrategy()
{
}

template<class Data>
inline void DiagonalSortingStrategy<Data>::sort(DataHolderT<Data>* data)
{
}

template<>
inline void DiagonalSortingStrategy<DataHolder*>::sort(DataHolderT<DataHolder*>* data)
{
	std::shared_ptr<RectangleCreator> rectCreator = data->getData()->getRectCreator();
	std::vector<RectangleHolder*>* rects = rectCreator->getRectList();
	auto cmp = [](RectangleHolder* r1, RectangleHolder* r2) {
		return r1->getRect().width() * r1->getRect().width() + r1->getRect().height() * r1->getRect().height() >
			r2->getRect().width() * r2->getRect().width() + r2->getRect().height() * r2->getRect().height(); };
	std::sort(rects->begin(), rects->end(), cmp);
}
