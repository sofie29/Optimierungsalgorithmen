#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Optimierungsalgorithmen.h"
#include "LocalSearch.h"
#include "RuleBasedNeighbour.h"
#include "GeometryBasedNeighbour.h"

class Optimierungsalgorithmen : public QMainWindow
{
    Q_OBJECT

public:
    Optimierungsalgorithmen(QWidget *parent = Q_NULLPTR);
    ~Optimierungsalgorithmen();

public slots:
    void RunOptim();
    void changeAlgorithm(int idx);
private:
    Ui::OptimierungsalgorithmenClass ui;
    QDockWidget* leftDock_;

    class MainScene* mainScene_;
    class MainWindow* mainWindow_;
    class AlgorithmSelectionUI* algoSelectionUI_;

    std::shared_ptr<class DataHolder> dataHolder_;
    std::shared_ptr<class DataHolder> bestDataHolder_;

    class OptimAlgoI* selectedAlgorithm_;
    LocalSearch<class DataHolder*>* localSearch_;

    RuleBasedNeighbour<class DataHolder*>* ruleBasedNeighbour_;
    InitialSolutionI<DataHolder*>* initSol_;
    class QNeighbourWrapper* ruleBasedWrapped_;

};
