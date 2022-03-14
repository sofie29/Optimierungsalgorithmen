#pragma once
#include "stdafx.h"

class RectangleCreator : public QWidget {
	Q_OBJECT
public:
	RectangleCreator();
	void getRectList(std::vector<QRectF>& list);
signals:
	void RectListCreated();
	void RectListUpdated(const std::vector<QRectF>& list);
public slots:
	void CreateRects(const int amount, const int maxEdgeLength);
	void OnRectPositionsUpdated(const std::vector<QRectF>& list);
private:
	std::vector<QRectF> rectangleList;

};