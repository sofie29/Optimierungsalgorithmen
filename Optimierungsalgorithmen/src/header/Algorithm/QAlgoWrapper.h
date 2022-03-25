#pragma once
#include "stdafx.h"
#include "OptimAlgoI.h"
#include "DataHolder.h"
class QAlgoWrapper : public QObject {
	Q_OBJECT
public:
	QAlgoWrapper(OptimAlgoI<DataHolder*>* alg);
	class OptimAlgoI<DataHolder*>* getAlgo();
	void setAlgorithm(OptimAlgoI<DataHolder*>* alg);
public slots:
	void RunUntilTermination();
	void RunOneStep();
	void Reset();
private:
	class OptimAlgoI<DataHolder*>* algo_;
};