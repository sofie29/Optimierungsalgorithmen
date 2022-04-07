#pragma once 
#include "stdafx.h"
#include "DataHolderT.h"
#include "InitialSolutionI.h"
#include "OptimAlgoI.h"
#include "LocalSearch.h"
#include "RuleBasedNeighbour.h"
#include "GeometryBasedNeighbour.h"
#include "ObjectiveI.h"
#include "EmptyBoxObjective.h"
#include "SimpleEmptyBoxObjective.h"
#include "BoxListLengthObjective.h"
#include "Greedy.h"
#include "AreaSortingStrategy.h"
#include "DiagonalSortingStrategy.h"
#include "GeometryBasedOverlappingNeighbour.h"
class TestEnvironment {
public:
	TestEnvironment(int rectAmount, int boxLength);
	TestEnvironment(int instances, int rect_amount, int min_rect_width, int min_rect_height, int max_rect_width, int max_rect_height, int boxLenght);
	~TestEnvironment();
	void Run(std::string path);
private:
	void Protocoll(std::ofstream& file, float score, float time);
	void ProtocollNewLine(std::ofstream& file, int prefix);
	void setAlgorithm(int index);
	int instances_;
	

	std::shared_ptr<class RectangleCreator> rectCreator_;
	std::shared_ptr<class BoundingBoxCreator> boxCreator_;
	DataHolderT<class DataHolder*>* bestDataT_;
	DataHolderT<class DataHolder*>* dataT_;
	class DataHolder* bestData_;
	class DataHolder* data_;

	InitialSolutionI<DataHolder*>* initSol_;
	

	OptimAlgoI<class DataHolder*>* selectedAlgorithm_;
	LocalSearch<class DataHolder*>* localSearch_;
	Greedy<class DataHolder*>* greedy_;


	EmptyBoxObjective<class DataHolder*>* emptyBoxObjective_;
	SimpleEmptyBoxObjective<class DataHolder*>* simpleEmptyBoxObjective_;
	BoxListLengthObjective<class DataHolder*>* boxListLengthObjective_;

	AreaSortingStrategy<class DataHolder*>* areaSortStrategy_;
	DiagonalSortingStrategy<class DataHolder*>* diagonalSortStrategy_;
	RuleBasedNeighbour<class DataHolder*>* ruleBasedNeighbour_;
	GeometryBasedNeighbour<class DataHolder*>* geometryBasedNeighbour_;
	GeometryBasedOverlappingNeighbour<class DataHolder*>* geometryBasedOverlappingNeighbour_;


	std::vector<int> min_rect_width_vector_;
	std::vector<int> max_rect_width_vector_;
	std::vector<int> min_rect_height_vector_;
	std::vector<int> max_rect_height_vector_;
	int rectAmount_;
	
};