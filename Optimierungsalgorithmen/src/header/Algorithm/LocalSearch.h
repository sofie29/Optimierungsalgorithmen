#pragma once
#include "OptimAlgoI.h"
#include "NeighbourI.h"
#include "InitialSolutionI.h"
#include "stdafx.h"
template<class Data>
class LocalSearch : public OptimAlgoI<Data>{

public:
	LocalSearch(NeighbourI<Data>* n, DataHolderT<Data>* sol, DataHolderT<Data>* bestSol, InitialSolutionI<Data>* initSol);
	~LocalSearch();
	//returns optimal box amount
	virtual float execute(int steps) override;
	virtual void reset() override;
	void setNeighbourDefinition(NeighbourI<Data>* n);
private:
	NeighbourI<Data>* neighbourDefinition_;
	//Data solution;
	float currentBestScore_;
	
};

template<class Data>
inline LocalSearch<Data>::LocalSearch(NeighbourI<Data>* n, DataHolderT<Data>* sol, DataHolderT<Data>* bestSol, InitialSolutionI<Data>* initSol) : OptimAlgoI<Data>(sol, bestSol, initSol)
{
	neighbourDefinition_ = n;
	currentBestScore_ = AlgorithmConstants::maxScore;
}

template<class Data>
LocalSearch<Data>::~LocalSearch()
{
	neighbourDefinition_ = nullptr;
	//solution = nullptr;
}


template<class Data>
float LocalSearch<Data>::execute(int steps)
{
	auto t1 = std::chrono::high_resolution_clock::now();

	if (OptimAlgoI<Data>::currentStep_ == -1) {
		OptimAlgoI<Data>::initSol_->CreateInitialSolution(OptimAlgoI<Data>::currentSol_);
		OptimAlgoI<Data>::bestSol_->OverwriteData(OptimAlgoI<Data>::currentSol_);
		neighbourDefinition_->resetData();
		OptimAlgoI<Data>::currentStep_++;
	}

	// TODO: RESET PARAMETER T 
	int steps_left = 0;
	while(OptimAlgoI<Data>::currentTimeTaken_ < AlgorithmConstants::maxTime_- AlgorithmConstants::timeOverhead_ && steps_left < steps){
		std::cout << "Iteration: " << OptimAlgoI<Data>::currentStep_ << std::endl;
		float tmp = neighbourDefinition_->optimize();
		if (tmp < currentBestScore_) {
			currentBestScore_ = tmp;
			OptimAlgoI<Data>::bestSol_->OverwriteData(OptimAlgoI<Data>::currentSol_);
		}
		//OptimAlgoI<Data>::currentSol_->ResetData();
		neighbourDefinition_->resetData();

		std::cout << "" << std::endl;
		OptimAlgoI<Data>::currentStep_++;
		emit OptimAlgoI<Data>::EmitCurrentStep(OptimAlgoI<Data>::currentStep_);
		
		steps_left++;

		auto t2 = std::chrono::high_resolution_clock::now();
		auto ms = std::chrono::duration<double, std::milli>(t2 - t1);
		OptimAlgoI<Data>::currentTimeTaken_ += ms.count();
		t1 = std::chrono::high_resolution_clock::now();
	}
	bool done = OptimAlgoI<Data>::currentStep_ >= AlgorithmConstants::maxIterations;
	if (!done) {
		std::cout << "Score after step: " << currentBestScore_ << std::endl;
		emit OptimAlgoI<Data>::StepDone();
		emit OptimAlgoI<Data>::DrawSolution();
	}
	else {
		std::cout << "Score after optimization: " << currentBestScore_ << std::endl;
		emit OptimAlgoI<Data>::OptimDone();
		emit OptimAlgoI<Data>::DrawSolution();
	}
	
	auto t2 = std::chrono::high_resolution_clock::now();
	auto ms = std::chrono::duration<double, std::milli>(t2 - t1);
	OptimAlgoI<Data>::currentTimeTaken_ += ms.count();
	emit OptimAlgoI<Data>::EmitTakenTime(OptimAlgoI<Data>::currentTimeTaken_);
	emit OptimAlgoI<Data>::EmitTakenTimeAvg(OptimAlgoI<Data>::currentTimeTaken_ /(double) OptimAlgoI<Data>::currentStep_);
	return currentBestScore_;
}

template<class Data>
inline void LocalSearch<Data>::reset()
{

	OptimAlgoI<Data>::currentSol_->ResetData();
	OptimAlgoI<Data>::bestSol_->ResetData();
	currentBestScore_ = AlgorithmConstants::maxScore;;
	OptimAlgoI<Data>::currentStep_ = 0;
	OptimAlgoI<Data>::currentTimeTaken_ = 0.0;

	auto t1 = std::chrono::high_resolution_clock::now();

	OptimAlgoI<Data>::initSol_->CreateInitialSolution(OptimAlgoI<Data>::currentSol_);
	OptimAlgoI<Data>::bestSol_->OverwriteData(OptimAlgoI<Data>::currentSol_);
	neighbourDefinition_->resetData();
	emit OptimAlgoI<Data>::EmitCurrentStep(OptimAlgoI<Data>::currentStep_);
	emit OptimAlgoI<Data>::StepDone();
	emit OptimAlgoI<Data>::DrawSolution();

	auto t2 = std::chrono::high_resolution_clock::now();
	auto ms = std::chrono::duration<double, std::milli>(t2 - t1);
	OptimAlgoI<Data>::currentTimeTaken_ += ms.count();
	emit OptimAlgoI<Data>::EmitTakenTime(OptimAlgoI<Data>::currentTimeTaken_);
	emit OptimAlgoI<Data>::EmitTakenTimeAvg(OptimAlgoI<Data>::currentTimeTaken_);
}

template<class Data>
inline void LocalSearch<Data>::setNeighbourDefinition(NeighbourI<Data>* n)
{
	neighbourDefinition_ = n;
}



