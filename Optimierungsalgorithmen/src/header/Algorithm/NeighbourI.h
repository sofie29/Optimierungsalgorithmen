#pragma once
#include "stdafx.h"
#include "InitialSolutionI.h"
#include "DataHolderT.h"

template<class Data>
class NeighbourI {

public:
	NeighbourI(DataHolderT<Data>* d, DataHolderT<Data>* currentBest,  InitialSolutionI<Data>* initSol);
	
	~NeighbourI();
	virtual float optimize() = 0;
	virtual void resetData() = 0;
	virtual void initParameters() = 0;
	virtual void afterOptimization() = 0;
	virtual void postOptimStep(float newScore, float oldScore) = 0;

	std::string getIdentifier();
	
protected:
	DataHolderT<Data>* data_;
	DataHolderT<Data>* bestData_;
	InitialSolutionI<Data>* initSol_;
	std::string identifier_;
};

template<class Data>
inline NeighbourI<Data>::NeighbourI(DataHolderT<Data>* d, DataHolderT<Data>* currentBest, InitialSolutionI<Data>* initSol) : data_(d), bestData_(currentBest), initSol_(initSol)
{
	// TODO: reset when changing parameters (e.g. number of rectangles)
	// currentBest_ = 999000; // worst score in GeometryBasedNeighbourhood with 1000 rectangles
	//initSol_->CreateInitialSolution(d);
	identifier_ = "";
}


template<class Data>
inline NeighbourI<Data>::~NeighbourI()
{
}

template<class Data>
inline std::string NeighbourI<Data>::getIdentifier()
{
	return identifier_;
}
