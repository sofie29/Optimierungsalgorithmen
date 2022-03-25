#pragma once
#include "stdafx.h"
#include "InitialSolutionI.h"
#include "DataHolderT.h"
class SignalHelper : public QObject {
	Q_OBJECT
signals:
	void OptimDone();
	void StepDone();
	void DrawSolution();
	void EmitCurrentStep(int step);

};
template<class Data>
class OptimAlgoI : public SignalHelper{
public:
	OptimAlgoI(DataHolderT<Data>* dt, DataHolderT<Data>* bestSol, class InitialSolutionI<Data>* init);
	virtual float execute(int steps) = 0;
	virtual void reset() = 0;
protected:
	int currentStep_;
	DataHolderT<Data>* currentSol_;
	DataHolderT<Data>* bestSol_;
	class InitialSolutionI<Data>* initSol_;
};


template<class Data>
inline OptimAlgoI<Data>::OptimAlgoI(DataHolderT<Data>* dt, DataHolderT<Data>* bestSol, InitialSolutionI<Data>* init)
{
	currentStep_ = -1;
	currentSol_ = dt;
	bestSol_ = bestSol;
	initSol_ = init;
}
