#pragma once
#include "ObjectiveI.h"
#include "DataHolderT.h"
#include "DataHolder.h"
#include "BoundingBox.h"
#include "BoundingBoxCreator.h"
template<class Data>
class BoxListLengthObjective : public ObjectiveI<Data> {
public:
	BoxListLengthObjective();
	virtual float calculateObjectiveScore(DataHolderT<Data>* dt) override;

};

template<class Data>
inline BoxListLengthObjective<Data>::BoxListLengthObjective() : ObjectiveI<Data>()
{
}

template<class Data>
inline float BoxListLengthObjective<Data>::calculateObjectiveScore(DataHolderT<Data>* dt)
{
	return 0.0f;
}

template<>
inline float BoxListLengthObjective<DataHolder*>::calculateObjectiveScore(DataHolderT<DataHolder*>* dt)
{
	std::vector<std::shared_ptr<BoundingBox>> boxList;
	dt->getData()->getBoxCreator()->getBoundingBoxList(boxList);
	return (float) boxList.size();
}