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
signals:
	void RectListUpdated(const std::vector<class RectangleHolder* >* list);
	void EmitRectList(const std::vector<class RectangleHolder* >* list);
	void EmitSquareSize(float size);
public slots:
	void OnOptimDone();
	void CreateRects(const int amount, const int maxEdgeLength);
private:
	std::vector<class RectangleHolder*>* rectangleList_;

};