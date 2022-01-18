#pragma once
#include "stdafx.h"
class RectangleDrawer : public QWidget {
	Q_OBJECT
public:
	RectangleDrawer(QGraphicsScene* scene);
	~RectangleDrawer();
public slots:
	void DrawRectAmountChangedI(const int amount);
	void DrawRectAmountChangedS(const QString& amount);
	void DrawRectSizeChangedI(const int maxEdgeLength);
	void DrawRectSizeChangedS(const QString& maxEdgeLength);
private:
	QGraphicsScene* scene_;
	int currentAmount_;
	int currentMaxEdgeLength_;

	void DrawRects();
};