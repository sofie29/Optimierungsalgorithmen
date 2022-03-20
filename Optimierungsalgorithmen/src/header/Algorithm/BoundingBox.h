#pragma once
#include "stdafx.h"

class BoundingBox {
public:
	BoundingBox(int rect_width, int rect_height, int x_pos, int y_pos, QRectF& rect); //rect_h/w bounding box, QRectF interior rect
	BoundingBox(int rect_width, int rect_height, int x, int y);
	//static void add(int rect_width, int rect_height);
	bool tryFit(int rect_width, int rect_height, int& x_out, int& y_out);
	int getXPos() const;
	int getYPos() const;
private:
	//static std::vector<BoundingBox*> solution;
	int rect_width, rect_height;
	int x, y; //global x, y wrt parent bounding box 
	std::shared_ptr<BoundingBox> first;
	std::shared_ptr<BoundingBox> second;

};