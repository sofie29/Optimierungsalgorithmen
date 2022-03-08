#pragma once
#include "NeighbourI.h"
template<class Solution>
class RuleBasedNeighbour : public NeighbourI<Solution> {
public:
	RuleBasedNeighbour();
	virtual void optimize();
};

template<class Solution>
inline RuleBasedNeighbour<Solution>::RuleBasedNeighbour()
{
}

template<class Solution>
inline void RuleBasedNeighbour<Solution>::optimize()
{
}
