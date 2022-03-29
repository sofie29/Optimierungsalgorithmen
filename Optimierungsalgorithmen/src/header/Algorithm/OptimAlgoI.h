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
	void EmitTakenTime(double time);
	void EmitTakenTimeAvg(double time);

};
template<class Data>
class OptimAlgoI : public SignalHelper{
public:
	OptimAlgoI(DataHolderT<Data>* dt, DataHolderT<Data>* bestSol, class InitialSolutionI<Data>* init);
	virtual float execute(int steps) = 0;
	virtual void reset() = 0;
	std::string getIdentifier();
protected:
	int currentStep_;
	DataHolderT<Data>* currentSol_;
	DataHolderT<Data>* bestSol_;
	class InitialSolutionI<Data>* initSol_;
	double currentTimeTaken_;
	std::string identifier_;
};


template<class Data>
inline OptimAlgoI<Data>::OptimAlgoI(DataHolderT<Data>* dt, DataHolderT<Data>* bestSol, InitialSolutionI<Data>* init)
{
	identifier_ = "";
	currentStep_ = -1;
	currentTimeTaken_ = 0.0;
	currentSol_ = dt;
	bestSol_ = bestSol;
	initSol_ = init;
}

template<class Data>
inline std::string OptimAlgoI<Data>::getIdentifier()
{
	return identifier_;
}
