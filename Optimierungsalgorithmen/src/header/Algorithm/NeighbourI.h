#pragma once
#include "stdafx.h"
#include "InitialSolutionI.h"
class SignalHelper : public QWidget {
	Q_OBJECT
signals:
	void OptimDone();

};

template<class Data>
class NeighbourI : public SignalHelper  {

public:
	NeighbourI(Data d, InitialSolutionI<Data>* initSol);
	
	~NeighbourI();
	virtual void optimize() = 0;
	inline int getBestScore() { return currentBest_; }
protected:
	int currentBest_;
	Data data_;
	InitialSolutionI<Data>* initSol_;
	
};

template<class Data>
inline NeighbourI<Data>::NeighbourI(Data d, InitialSolutionI<Data>* initSol) : data_(d), initSol_(initSol)
{
	currentBest_ = 0;
	initSol_->CreateInitialSolution(d);
}


template<class Data>
inline NeighbourI<Data>::~NeighbourI()
{
}
