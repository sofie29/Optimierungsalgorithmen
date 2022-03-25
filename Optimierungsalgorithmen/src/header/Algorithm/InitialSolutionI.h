
#pragma once
#include "stdafx.h"
#include "DataHolderT.h"
template<class Data>
class InitialSolutionI {
public:
	InitialSolutionI();
	virtual void CreateInitialSolution(DataHolderT<Data>* data) = 0;
};

template<class Data>
inline InitialSolutionI<Data>::InitialSolutionI()
{
}

template<class Data>
inline void InitialSolutionI<Data>::CreateInitialSolution(DataHolderT<Data>* data)
{
}

