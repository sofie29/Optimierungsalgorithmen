#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Optimierungsalgorithmen.h"
#include "LocalSearch.h"
#include "RuleBasedNeighbour.h"
#include "GeometryBasedNeighbour.h"
#include "GeometryBasedOverlappingNeighbour.h"
#include "OptimAlgoI.h"
#include "DataHolderT.h"
#include "Greedy.h"
#include "AreaSortingStrategy.h"
#include "DiagonalSortingStrategy.h"
#include "ObjectiveI.h"
#include "EmptyBoxObjective.h"
#include "SimpleEmptyBoxObjective.h"
#include "BoxListLengthObjective.h"
class Optimierungsalgorithmen : public QMainWindow
{
    Q_OBJECT

public:
    Optimierungsalgorithmen(QWidget* parent = Q_NULLPTR);
    ~Optimierungsalgorithmen();

public slots:
    void changeAlgorithm(int idx);
private:
    Ui::OptimierungsalgorithmenClass ui;
    QDockWidget* leftDock_;

    class MainScene* mainScene_;
    class MainWindow* mainWindow_;
    class AlgorithmSelectionUI* algoSelectionUI_;

    DataHolderT<class DataHolder*>* dataHolderT_;
    DataHolderT<class DataHolder*>* bestDataHolderT_;

    class DataHolder* dataHolder_;
    class DataHolder* bestDataHolder_;

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
    InitialSolutionI<DataHolder*>* initSol_;
    class QNeighbourWrapper* neighbourWrapper_;
    class QAlgoWrapper* algoWrapper_;

};
