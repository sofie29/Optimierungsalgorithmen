#pragma once
#include "stdafx.h"

class SignalHelper : public QObject {
	Q_OBJECT
signals:
	void OptimDone();
	void DrawSolution();

};
class OptimAlgoI : public SignalHelper{
public:
	OptimAlgoI();
	virtual float execute() = 0;
};