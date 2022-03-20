#pragma once
#include "stdafx.h"

class BoundingBox {
public:
	BoundingBox(int rect_width, int rect_height, int x_pos, int y_pos, QRectF& rect, int rectIndex); //rect_h/w bounding box, QRectF interior rect
	BoundingBox(int rect_width, int rect_height, int x, int y);
	//static void add(int rect_width, int rect_height);
	bool tryFit(int rect_width, int rect_height, int& x_out, int& y_out); // don't forget to set rectangle indices after calling tryFit
	std::vector<int> getRectangleIndices();
	void addRectangleIndex(int index);
	void removeRectangleIndex(int index);

private:
	//static std::vector<BoundingBox*> solution;
	int rect_width, rect_height;
	int x, y; //global x, y wrt parent bounding box 
	std::shared_ptr<BoundingBox> first;
	std::shared_ptr<BoundingBox> second;
	std::vector<int> rectangleIndices; // only for the top level bounding boxes: indices of all rectangles placed withing this bounding box
	int numberOfRectangles; // for objective function
};