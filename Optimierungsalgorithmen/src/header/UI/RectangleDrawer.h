#pragma once
#include "stdafx.h"
class RectangleDrawer : public QWidget {
	Q_OBJECT
public:
	RectangleDrawer();
	~RectangleDrawer();
	void DrawOnScreen(QGraphicsScene* scene);
signals:
	void EmitRectInformation(const int amount, const int edgeLength);
	void EmitListChanged();
public slots:
	void DrawRectAmountChangedI(const int amount);
	void DrawRectAmountChangedS(const QString& amount);
	void DrawRectSizeChangedI(const int maxEdgeLength);
	void DrawRectSizeChangedS(const QString& maxEdgeLength);
	void SetRects(const std::vector<class RectangleHolder*>* list);
	void OnBoundingBoxSizeChanged(int x);
private:
	int currentAmount_;
	int currentMaxEdgeLength_;
	std::vector<QRectF> rectList_;
	std::vector<QColor> rectColors_;
	
};