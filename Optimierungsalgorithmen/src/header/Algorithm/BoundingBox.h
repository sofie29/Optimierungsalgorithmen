#pragma once
#include "stdafx.h"
class BoundingBox {
public:
	BoundingBox(int rect_height, int rect_width);
	static void add(int rect_width, int rect_height);
private:
	bool tryFit(int rect_width, int rect_height);
	static std::vector<BoundingBox*> solution;
	int rect_width, rect_height; 
	int x, y; //global x, y wrt parent bounding box 
	BoundingBox* first;
	BoundingBox* second;

	
};