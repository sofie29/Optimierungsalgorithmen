#pragma once
#include "stdafx.h"

class RectangleCreator : public QWidget {
	Q_OBJECT
public:
	RectangleCreator();
	~RectangleCreator();
	std::vector<class RectangleHolder*>* getRectList();
	void setNewRectList(std::vector<class RectangleHolder*>* list);
	void setRectList(std::vector<class RectangleHolder* >* list);
	void ResetData();
	float getSquareSize();
signals:
	void RectListUpdated(const std::vector<class RectangleHolder* >* list);
	void EmitRectList(const std::vector<class RectangleHolder* >* list);
	void EmitSquareSize(float size);
public slots:
	void OnOptimDone();
	void CreateRectsQ(const int amount, const int maxEdgeLength);
	void CreateRects(const int amount, const int minWidth, const int maxWidth, const int minHeight, const int maxHeight);
private:
	std::vector<class RectangleHolder*>* rectangleList_;
	float squareSize_;
};