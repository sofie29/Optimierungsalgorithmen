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
	void ResetRectColors();
	float getSquareSize();
	void ResetRectsForTestEnv(const int amount, const int minWidth, const int maxWidth, const int minHeight, const int maxHeight);
	void ShuffleList();
signals:
	void RectListUpdated(const std::vector<class RectangleHolder* >* list);
	void EmitRectList(const std::vector<class RectangleHolder* >* list);
	void EmitSquareSize(float size);
public slots:
	void OnOptimDone();
	
	void CreateRects(const int amount, const int minWidth, const int maxWidth, const int minHeight, const int maxHeight);
private:
	std::vector<class RectangleHolder*>* rectangleList_;
	float squareSize_;

	int amount_;
	int minWidth_; 
	int maxWidth_; 
	int minHeight_;
	int maxHeight_;
};