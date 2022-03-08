#pragma once

template<class Solution>
class NeighbourI {
public:
	virtual void optimize() = 0;
	inline int getBestScore() { return currentBest; }
private:
	int currentBest;
	Solution* solution; 
	
};