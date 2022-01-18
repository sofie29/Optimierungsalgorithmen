#pragma once
#include "stdafx.h"
class MainWindow : public QGraphicsView {
	Q_OBJECT
public:
	MainWindow(QMainWindow* win, QGraphicsScene* scene);
	~MainWindow();

};