#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Optimierungsalgorithmen.h"

class Optimierungsalgorithmen : public QMainWindow
{
    Q_OBJECT

public:
    Optimierungsalgorithmen(QWidget *parent = Q_NULLPTR);
    ~Optimierungsalgorithmen();

private:
    Ui::OptimierungsalgorithmenClass ui;
    QDockWidget* leftDock_;

    class MainScene* mainScene_;
    class MainWindow* mainWindow_;
    class AlgorithmSelectionUI* algoSelectionUI_;
    
};
