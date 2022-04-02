#pragma once
#include "stdafx.h"
#include "DataHolderT.h"
template<class Data>
class SortingStrategyI {
public:
	SortingStrategyI();
	virtual void sort(DataHolderT<Data>* data) = 0;

};

template<class Data>
inline SortingStrategyI<Data>::SortingStrategyI()
{
}
