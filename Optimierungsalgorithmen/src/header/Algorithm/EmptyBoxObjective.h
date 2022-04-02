#pragma once
#include "stdafx.h"
#include "ObjectiveI.h"
#include "DataHolderT.h"
#include "DataHolder.h"
#include "BoundingBox.h"
#include "BoundingBoxCreator.h"
#include "RectangleCreator.h"

template<class Data>
class EmptyBoxObjective : public ObjectiveI<Data> {
public:
	EmptyBoxObjective();
	virtual float calculateObjectiveScore(DataHolderT<Data>* dt) override;
	void setNeighbour(NeighbourI<Data>* neighbour);

private:
	NeighbourI<Data>* neighbour_;
};

template<class Data>
inline EmptyBoxObjective<Data>::EmptyBoxObjective()
{
}

template<class Data>
inline float EmptyBoxObjective<Data>::calculateObjectiveScore(DataHolderT<Data>* dt)
{
	return 0.0f;
}

template<class Data>
inline void EmptyBoxObjective<Data>::setNeighbour(NeighbourI<Data>* neighbour)
{
	neighbour_ = neighbour;
}

template<>
inline float EmptyBoxObjective<DataHolder*>::calculateObjectiveScore(DataHolderT<DataHolder*>* dt)
{
	std::vector<std::shared_ptr<BoundingBox>> bBoxList;
	dt->getData()->getBoxCreator()->getBoundingBoxList(bBoxList);
	size_t bBoxListSize = bBoxList.size();
	size_t rectListSize = dt->getData()->getRectCreator()->getRectList()->size();

	float average = static_cast<float>(rectListSize) / static_cast<float>(bBoxListSize);
	float boxScore = 0;

	for (std::shared_ptr<BoundingBox> box : bBoxList) {
		float rectanglesInBox = static_cast<float>((box->getRectangleIndices()).size());

		if (average > rectanglesInBox) {
			boxScore += (average - rectanglesInBox) / average; // the emptier a box is, the better it is
		}

	}

	return ((bBoxListSize + neighbour_->getScoreInformation()) * (rectListSize - 1) - boxScore) * 0.2; // 0.2 to prevent overflow
}