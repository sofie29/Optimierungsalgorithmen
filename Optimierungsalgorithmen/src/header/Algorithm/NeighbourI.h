#pragma once
#include "stdafx.h"
#include "InitialSolutionI.h"


template<class Data>
class NeighbourI {

public:
	NeighbourI(Data d, Data currentBest,  InitialSolutionI<Data>* initSol);
	
	~NeighbourI();
	virtual int optimize() = 0;
	
protected:
	Data data_;
	Data bestData_;
	InitialSolutionI<Data>* initSol_;
	
};

template<class Data>
inline NeighbourI<Data>::NeighbourI(Data d, Data currentBest, InitialSolutionI<Data>* initSol) : data_(d), bestData_(currentBest), initSol_(initSol)
{
	// TODO: reset when changing parameters (e.g. number of rectangles)
	// currentBest_ = 999000; // worst score in GeometryBasedNeighbourhood with 1000 rectangles
	initSol_->CreateInitialSolution(d);
}


template<class Data>
inline NeighbourI<Data>::~NeighbourI()
{
}
