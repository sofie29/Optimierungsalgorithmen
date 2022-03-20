#pragma once
#include "stdafx.h"

class RectangleCreator : public QWidget {
	Q_OBJECT
public:
	RectangleCreator();
	void getRectList(std::vector<QRectF>& list);
	void setNewRectList(std::vector<QRectF>& list);
	void setRectList(std::vector<QRectF>& list);
signals:
	void RectListUpdated(const std::vector<QRectF>& list);
	void EmitRectList(const std::vector<QRectF>& list);
	void EmitSquareSize(float size);
public slots:
	void OnOptimDone();
	void CreateRects(const int amount, const int maxEdgeLength);
private:
	std::vector<QRectF> rectangleList_;

};