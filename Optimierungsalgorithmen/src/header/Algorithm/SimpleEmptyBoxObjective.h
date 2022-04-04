#pragma once
#include "ObjectiveI.h"
#include "DataHolder.h"
#include "DataHolderT.h"
#include "BoundingBox.h"
#include "BoundingBoxCreator.h"
#include "RectangleCreator.h"
template<class Data>
class SimpleEmptyBoxObjective : public ObjectiveI<Data> {
public:
	SimpleEmptyBoxObjective();
	virtual float calculateObjectiveScore(DataHolderT<Data>* dt) override;

};

template<class Data>
inline SimpleEmptyBoxObjective<Data>::SimpleEmptyBoxObjective() : ObjectiveI<Data>()
{
}

template<class Data>
inline float SimpleEmptyBoxObjective<Data>::calculateObjectiveScore(DataHolderT<Data>* dt)
{
	return 0.0f;
}

template<>
inline float SimpleEmptyBoxObjective<DataHolder*>::calculateObjectiveScore(DataHolderT<DataHolder*>* dt)
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

	// upper bound of boxScore : length of rectangleList - 1
	return (bBoxListSize * (rectListSize - 1) - boxScore) * 0.2; // 0.2 to prevent overflow
}
