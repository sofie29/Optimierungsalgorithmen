#pragma once
#include "stdafx.h"
#include "OptimAlgoI.h"
#include "SortingStrategyI.h"
#include "InitialSolutionI.h"
template<class Data>
class Greedy : public OptimAlgoI<Data> {
public:
	Greedy(DataHolderT<Data>* dt, DataHolderT<Data>* bestSol, InitialSolutionI<Data>* initSol, SortingStrategyI<Data>* sortStrat, ObjectiveI<Data>* algoObjective, ObjectiveI<Data>* cmpObjective);
	virtual Metric execute(int steps) override;
	virtual void reset() override;
	void setSortStrat(SortingStrategyI<Data>* sortStrat);
private:
	SortingStrategyI<Data>* sortStrategy_;
};

template<class Data>
inline Greedy<Data>::Greedy(DataHolderT<Data>* dt, DataHolderT<Data>* bestSol, InitialSolutionI<Data>* initSol, SortingStrategyI<Data>* sortStrat, ObjectiveI<Data>* algoObjective, ObjectiveI<Data>* cmpObjective)
	: OptimAlgoI<Data>(dt, bestSol, initSol, algoObjective, cmpObjective), sortStrategy_(sortStrat)
{
	OptimAlgoI<Data>::identifier_ = "Greedy " + sortStrategy_->getIdentifier();
}

template<class Data>
inline Metric Greedy<Data>::execute(int steps)
{
	OptimAlgoI<Data>::currentSol_->SoftResetData();
	auto t1 = std::chrono::high_resolution_clock::now();

	sortStrategy_->sort(OptimAlgoI<Data>::currentSol_);
	OptimAlgoI<Data>::initSol_->CreateInitialSolution(OptimAlgoI<Data>::currentSol_, false);
	OptimAlgoI<Data>::currentStep_ += 1;
	OptimAlgoI<Data>::currentBestScore_ = OptimAlgoI<Data>::algoObjective_->calculateObjectiveScore(OptimAlgoI<Data>::currentSol_);
	OptimAlgoI<Data>::bestSol_->OverwriteData(OptimAlgoI<Data>::currentSol_);
	emit OptimAlgoI<Data>::StepDone();
	emit OptimAlgoI<Data>::EmitCurrentStep(OptimAlgoI<Data>::currentStep_);

	auto t2 = std::chrono::high_resolution_clock::now();
	auto ms = std::chrono::duration<double, std::milli>(t2 - t1);
	OptimAlgoI<Data>::currentTimeTaken_ += ms.count();
	emit OptimAlgoI<Data>::EmitTakenTime(OptimAlgoI<Data>::currentTimeTaken_);
	emit OptimAlgoI<Data>::EmitTakenTimeAvg(OptimAlgoI<Data>::currentTimeTaken_ / (double)OptimAlgoI<Data>::currentStep_);

	//emit OptimAlgoI<Data>::DrawSolution();
	return Metric::Metric(OptimAlgoI<Data>::cmpObjective_->calculateObjectiveScore(OptimAlgoI<Data>::currentSol_) , OptimAlgoI<Data>::currentTimeTaken_);
}

template<class Data>
inline void Greedy<Data>::reset()
{
	OptimAlgoI<Data>::currentSol_->ResetData();
	OptimAlgoI<Data>::bestSol_->ResetData();
	OptimAlgoI<Data>::currentBestScore_ = AlgorithmConstants::maxScore;;
	OptimAlgoI<Data>::currentStep_ = 0;
	OptimAlgoI<Data>::currentTimeTaken_ = 0.0;

	OptimAlgoI<Data>::initSol_->CreateInitialSolution(OptimAlgoI<Data>::currentSol_, true);
	OptimAlgoI<Data>::bestSol_->OverwriteData(OptimAlgoI<Data>::currentSol_);
	emit OptimAlgoI<Data>::StepDone();
	//emit OptimAlgoI<Data>::DrawSolution();

	emit OptimAlgoI<Data>::EmitCurrentStep(OptimAlgoI<Data>::currentStep_);
	emit OptimAlgoI<Data>::EmitTakenTime(OptimAlgoI<Data>::currentTimeTaken_);
	emit OptimAlgoI<Data>::EmitTakenTimeAvg(OptimAlgoI<Data>::currentTimeTaken_);
}

template<class Data>
inline void Greedy<Data>::setSortStrat(SortingStrategyI<Data>* sortStrat)
{
	sortStrategy_ = sortStrat;
	OptimAlgoI<Data>::identifier_ = "Greedy " + sortStrategy_->getIdentifier();
}
