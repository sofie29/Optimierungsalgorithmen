#pragma once
#include "stdafx.h"
#include "NeighbourI.h"
class QNeighbourWrapper : public QWidget {
	Q_OBJECT
public:
	QNeighbourWrapper(class NeighbourI<class DataHolder*>* n);
	class NeighbourI<class DataHolder*>* getNeighbourI();
public slots:
	void Optim();
private:
	NeighbourI<class DataHolder*>* neighbour_;
};