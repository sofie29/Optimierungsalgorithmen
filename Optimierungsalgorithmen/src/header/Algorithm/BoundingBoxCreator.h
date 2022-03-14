#pragma once
#include "stdafx.h"

class BoundingBoxCreator : public QWidget {
	Q_OBJECT
public:
	BoundingBoxCreator();
	void getRectList(std::vector<QRectF>& list);
	void getBoundingBoxList(std::vector<std::shared_ptr<class BoundingBox>>& list);
	void addBoundingBox(int x_pos, int y_pos, QRectF& rect);
signals:
	void EmitRectList(std::vector<QRectF>& list);
public slots:
	void EdgeLengthChanged(const int length);
	void OnOptimDone();
	
private:
	std::vector<QRectF> rectangleList_;
	std::vector<std::shared_ptr<class BoundingBox>> boundingBoxList_;
	int edgeLength_;

};