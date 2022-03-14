
#pragma once
#include "stdafx.h"

template<class Data>
class InitialSolutionI {
public:
	InitialSolutionI();
	virtual void CreateInitialSolution(Data data) = 0;
};

template<class Data>
inline InitialSolutionI<Data>::InitialSolutionI()
{
}

template<class Data>
inline void InitialSolutionI<Data>::CreateInitialSolution(Data data)
{
}

