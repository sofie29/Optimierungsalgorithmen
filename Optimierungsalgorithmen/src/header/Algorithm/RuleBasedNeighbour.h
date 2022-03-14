#pragma once
#include "NeighbourI.h"
#include "DataHolder.h"
#include "BoundingBoxCreator.h"
#include "RectangleCreator.h"
#include "InitialSolutionI.h"
template<class Data>
class RuleBasedNeighbour : public NeighbourI<Data> {
public:
	RuleBasedNeighbour(Data data, InitialSolutionI<Data>* initSol);
	
	virtual void optimize();
};


template<class Data>
inline RuleBasedNeighbour<Data>::RuleBasedNeighbour(Data data, InitialSolutionI<Data>* initSol) : NeighbourI<Data>(data, initSol)
{
	//NeighbourI<Data>::data_ = data;
}


template<class Data>
inline void RuleBasedNeighbour<Data>::optimize()
{
}

template<>
inline void RuleBasedNeighbour<DataHolder*>::optimize() {
	
	initSol_->CreateInitialSolution(data_);
	emit OptimDone();

}