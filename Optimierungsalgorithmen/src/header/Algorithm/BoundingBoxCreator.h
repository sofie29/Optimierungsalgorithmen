#pragma once
#include "stdafx.h"
#include "RectangleHolder.h"

class BoundingBoxCreator : public QWidget {
	Q_OBJECT
public:
	BoundingBoxCreator();

	void getRectList(std::vector<QRectF>& list);
	void setRectList(std::vector<QRectF>& list);
	void getBoundingBoxList(std::vector<std::shared_ptr<class BoundingBox>>& list);
	void setBoundingBoxList(std::vector<std::shared_ptr<class BoundingBox>>& list);
	void ResetBoundingBoxList();
	void addBoundingBox(int x_pos, int y_pos, class RectangleHolder* rectHolder, int rectIndex, int boxIndex);
	void addBoundingBox(int x_pos, int y_pos, QRectF& rect, int rectIndex);
	void addBoundingBox(int x_pos, int y_pos, QRectF& rect);
	float getCurrentFitScore();
	void setCurrentFitScore(float score);
	
signals:
	void EmitNumberOfBoundingBoxes(int amount);
	void EmitCurrentFitScore(float score);
	void EmitOptimalAmountRect(float amount);
	void EmitRectList(std::vector<QRectF>& list);
public slots:
	void EdgeLengthChanged(const int length);
	void RecSquareSizeChanged(int rectSquareSize);
	void OnOptimDone();
	
private:
	std::vector<QRectF> rectangleList_; //this is the list for the bounding Box rectangles to draw them easily, NOT the actual rectangles
	std::vector<std::shared_ptr<class BoundingBox>> boundingBoxList_;
	int edgeLength_;
	float currentFitScore_;
	float optimalAmount_;
	int rectSquareSize_; //actual rec amount
};