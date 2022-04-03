#pragma once
#include "stdafx.h"
#include "DataHolderT.h"
template<class Data>
class SortingStrategyI {
public:
	SortingStrategyI();
	virtual void sort(DataHolderT<Data>* data) = 0;
	virtual std::string getIdentifier();
protected:
	std::string identifier_;

};

template<class Data>
inline SortingStrategyI<Data>::SortingStrategyI()
{
	identifier_ = "";
}

template<class Data>
inline std::string SortingStrategyI<Data>::getIdentifier()
{
	return identifier_;
}
