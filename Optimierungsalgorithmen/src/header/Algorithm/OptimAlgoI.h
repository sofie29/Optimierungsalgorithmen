#pragma once
#include "stdafx.h"
#include "InitialSolutionI.h"
#include "DataHolderT.h"
#include "ObjectiveI.h"
struct Metric {
public: Metric(float s, float t) {
	score_ = s;
	time_ = t;
}
	  float score_;
	  float time_;
};

class SignalHelper : public QObject {
	Q_OBJECT
signals:
	void OptimDone();
	void StepDone();
	void DrawSolution();
	void DrawSwappedRects();
	void EmitCurrentStep(int step);
	void EmitTakenTime(double time);
	void EmitTakenTimeAvg(double time);

};
template<class Data>
class OptimAlgoI : public SignalHelper{
public:
	OptimAlgoI(DataHolderT<Data>* dt, DataHolderT<Data>* bestSol, InitialSolutionI<Data>* init, ObjectiveI<Data>* algoObjective, ObjectiveI<Data>* cmpObjective);
	virtual Metric execute(int steps) = 0;
	virtual void reset() = 0;
	void setObjective(class ObjectiveI<Data>* objective);
	std::string getIdentifier();
protected:
	int currentStep_;
	float currentBestScore_;
	DataHolderT<Data>* currentSol_;
	DataHolderT<Data>* bestSol_;
	InitialSolutionI<Data>* initSol_;
	ObjectiveI<Data>* algoObjective_;
	ObjectiveI<Data>* cmpObjective_;
	double currentTimeTaken_;
	std::string identifier_;
};


template<class Data>
inline OptimAlgoI<Data>::OptimAlgoI(DataHolderT<Data>* dt, DataHolderT<Data>* bestSol, InitialSolutionI<Data>* init, ObjectiveI<Data>* algoObjective, ObjectiveI<Data>* cmpObjective)
{
	identifier_ = "";
	currentStep_ = -1;
	currentTimeTaken_ = 0.0;
	currentBestScore_ = AlgorithmConstants::maxScore;
	currentSol_ = dt;
	bestSol_ = bestSol;
	initSol_ = init;
	algoObjective_ = algoObjective;
	cmpObjective_ = cmpObjective;
}

template<class Data>
inline void OptimAlgoI<Data>::setObjective(ObjectiveI<Data>* objective)
{
	algoObjective_ = objective;
}

template<class Data>
inline std::string OptimAlgoI<Data>::getIdentifier()
{
	return identifier_;
}
