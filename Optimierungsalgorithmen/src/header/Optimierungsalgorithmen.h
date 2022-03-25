#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Optimierungsalgorithmen.h"
#include "LocalSearch.h"
#include "RuleBasedNeighbour.h"
#include "GeometryBasedNeighbour.h"
#include "OptimAlgoI.h"
#include "DataHolderT.h"
class Optimierungsalgorithmen : public QMainWindow
{
    Q_OBJECT

public:
    Optimierungsalgorithmen(QWidget *parent = Q_NULLPTR);
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

    RuleBasedNeighbour<class DataHolder*>* ruleBasedNeighbour_;
    GeometryBasedNeighbour<class DataHolder*>* geometryBasedNeighbour_;
    InitialSolutionI<DataHolder*>* initSol_;
    class QNeighbourWrapper* neighbourWrapper_;
    class QAlgoWrapper* algoWrapper_;

};
