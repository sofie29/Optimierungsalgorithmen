#pragma once
#include "OptimAlgoI.h"
#include "NeighbourI.h"
#include "stdafx.h"
template<class Solution>
class LocalSearch : public OptimAlgoI{

public:
	LocalSearch(NeighbourI<Solution>* n, Solution sol);
	~LocalSearch();
	//returns optimal box amount
	virtual float execute() override;
	void setNeighbourDefinition(NeighbourI<Solution>* n);
private:
	NeighbourI<Solution>* neighbourDefinition_;
	Solution solution;
	float currentBestScore_;
	
};

template<class Solution>
LocalSearch<Solution>::LocalSearch(NeighbourI<Solution>* n, Solution sol) : OptimAlgoI()
{
	neighbourDefinition_ = n;
	solution = sol;
	currentBestScore_ = AlgorithmConstants::maxScore; 
}

template<class Solution>
LocalSearch<Solution>::~LocalSearch()
{
	neighbourDefinition_ = nullptr;
	solution = nullptr;
}


template<class Solution>
float LocalSearch<Solution>::execute()
{

	for(int i = 0; i < AlgorithmConstants::maxIterations; i++){
		std::cout << "Iteration: " << i << std::endl;
		float tmp = neighbourDefinition_->optimize();
		if (tmp < currentBestScore_) {
			currentBestScore_ = tmp;
		}
		std::cout << "" << std::endl;
		
	}
	std::cout << "Score after optimization: " << currentBestScore_ << std::endl;

	emit OptimDone();
	emit DrawSolution();
	return currentBestScore_;
}

template<class Solution>
inline void LocalSearch<Solution>::setNeighbourDefinition(NeighbourI<Solution>* n)
{
	neighbourDefinition_ = n;
}



