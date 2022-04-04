#pragma once
#include "NeighbourI.h"
#include "DataHolder.h"
#include "BoundingBoxCreator.h"
#include "RectangleCreator.h"
#include "InitialSolutionI.h"
#include "BoundingBox.h"
#include "RectangleHolder.h"
#include "GeometryBasedNeighbourI.h"

template<class Data>
class GeometryBasedNeighbour : public GeometryBasedNeighbourI<Data> {
public:
	GeometryBasedNeighbour(DataHolderT<Data>* data, DataHolderT<Data>* currentBest, InitialSolutionI<Data>* initSol);
	virtual float optimize() override;
	virtual bool tryFitWrapper(std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIdx, std::vector<class RectangleHolder*>* rectangles, int rectIdx, bool multipleRects) override;
	virtual void initParameters() override;
	virtual int getRectPos(const int rectListSize, std::vector<std::shared_ptr<BoundingBox>>& boxList, bool& isTransgressionRect = false) override;
	virtual int getBoxPos(const int boxListSize, std::vector<std::shared_ptr<BoundingBox>>& boxList) override;
	virtual void resetBoxPos() override;

	// not implemented here
	virtual int calculateOverlappingWrapper(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) override;
	virtual void decreaseT(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, bool isBetter) override;
	virtual float getScoreInformation() override;
	virtual void beforeScoreCalculation(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, bool isTransgressionRect) override;


private:
	int rectPos_;
	int boxPos_;
};


template<class Data>
inline GeometryBasedNeighbour<Data>::GeometryBasedNeighbour(DataHolderT<Data>* data, DataHolderT<Data>* currentBest, InitialSolutionI<Data>* initSol) : GeometryBasedNeighbourI<Data>(data, currentBest, initSol)
{
	rectPos_ = 0;
	boxPos_ = -1;

	NeighbourI<Data>::identifier_ = "GeometryBasedNeighbour";
}


template<>
inline float GeometryBasedNeighbour<DataHolder*>::optimize() {
	return this->findNeighbour(true);
}


template<>
inline void GeometryBasedNeighbour<DataHolder*>::beforeScoreCalculation(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, bool isTransgressionRect) {
}


template<class Data>
inline bool GeometryBasedNeighbour<Data>::tryFitWrapper(std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIdx, std::vector<class RectangleHolder*>* rectangles, int rectIdx, bool multipleRects)
{
	return boxList[boxIdx]->tryFit((*rectangles)[rectIdx], boxIdx);
}


template<class Data>
inline void GeometryBasedNeighbour<Data>::initParameters()
{
	rectPos_ = 0;
}


template<class Data>
inline int GeometryBasedNeighbour<Data>::getRectPos(const int rectListSize, std::vector<std::shared_ptr<BoundingBox>>& boxList, bool& isTransgressionRect)
{
	rectPos_ = (rectPos_ >= rectListSize) ? 1 : rectPos_ + 1;
	return rectListSize - rectPos_;
}

template<class Data>
inline int GeometryBasedNeighbour<Data>::getBoxPos(const int boxListSize, std::vector<std::shared_ptr<BoundingBox>>& boxList)
{
	boxPos_ = boxPos_ < boxListSize - 1 ? boxPos_ + 1 : 0;
	return boxPos_;
}

template<class Data>
inline void GeometryBasedNeighbour<Data>::resetBoxPos()
{
	boxPos_ = -1;
}

template<class Data>
inline int GeometryBasedNeighbour<Data>::calculateOverlappingWrapper(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList)
{
	return 0;
}

template<class Data>
inline void GeometryBasedNeighbour<Data>::decreaseT(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, bool isBetter)
{
}


template<class Data>
inline float GeometryBasedNeighbour<Data>::getScoreInformation()
{
	return 0.0f;
}
