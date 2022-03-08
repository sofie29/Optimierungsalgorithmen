#pragma once
#include "NeighbourI.h"
#include "stdafx.h"
template<class Solution>
class LocalSearch{

public:
	LocalSearch(NeighbourI<Solution>* n, Solution* sol);
	~LocalSearch();
	//returns optimal box amount
	int execute();
private:
	NeighbourI<Solution>* neighbourDefinition;
	Solution* solution;
	
};

template<class Solution>
LocalSearch<Solution>::LocalSearch(NeighbourI<Solution>* n, Solution* sol)
{
	neighbourDefinition = n;
	solution = sol;
}

template<class Solution>
LocalSearch<Solution>::~LocalSearch()
{
	neighbourDefinition = nullptr;
	solution = nullptr;
}


template<class Solution>
int LocalSearch<Solution>::execute()
{

	for(int i = 0; i < AlgorithmConstants::maxIterations; i++){
		neighbourDefinition->optimize();
		
	}
	return neighbourDefinition->getBestScore();
}



