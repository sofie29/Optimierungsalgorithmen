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
	if (OptimAlgoI<Data>::currentStep_ == -1) {
		OptimAlgoI<Data>::initSol_->CreateInitialSolution(OptimAlgoI<Data>::currentSol_);
		OptimAlgoI<Data>::bestSol_->OverwriteData(OptimAlgoI<Data>::currentSol_);
		neighbourDefinition_->resetData();
		OptimAlgoI<Data>::currentStep_++;
	}

	int steps_left = 0;
	while(OptimAlgoI<Data>::currentStep_ < AlgorithmConstants::maxIterations && steps_left < steps){
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
	
	return currentBestScore_;
}

template<class Data>
inline void LocalSearch<Data>::reset()
{
	OptimAlgoI<Data>::currentSol_->ResetData();
	OptimAlgoI<Data>::bestSol_->ResetData();
	currentBestScore_ = AlgorithmConstants::maxScore;;
	OptimAlgoI<Data>::currentStep_ = 0;
	OptimAlgoI<Data>::initSol_->CreateInitialSolution(OptimAlgoI<Data>::currentSol_);
	OptimAlgoI<Data>::bestSol_->OverwriteData(OptimAlgoI<Data>::currentSol_);

	emit OptimAlgoI<Data>::EmitCurrentStep(OptimAlgoI<Data>::currentStep_ + 1);
	emit OptimAlgoI<Data>::StepDone();
	emit OptimAlgoI<Data>::DrawSolution();
}

template<class Data>
inline void LocalSearch<Data>::setNeighbourDefinition(NeighbourI<Data>* n)
{
	neighbourDefinition_ = n;
}



