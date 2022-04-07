#pragma once
#include "stdafx.h"

class BoundingBox {
public:
	BoundingBox(int rect_width, int rect_height, int x_pos, int y_pos, class RectangleHolder* rectHolder, int rectIndex, int boxIndex); //rect_h/w bounding box, QRectF interior rect
	BoundingBox(int rect_width, int rect_height, int x, int y);
	BoundingBox(int rect_width, int rect_height, int x, int y, std::vector<int> indices, int numberOv);
	BoundingBox(int rect_width, int rect_height, int x, int y, std::vector<int> indices, std::shared_ptr<BoundingBox> firstBox, std::shared_ptr<BoundingBox> secondBox, int numberOv); // for deep copy
	~BoundingBox();
	int getXPos() const;
	int getYPos() const;
	std::shared_ptr<BoundingBox> getFirstBox();
	std::shared_ptr<BoundingBox> getSecondBox();
	std::vector<int> getRectangleIndices();
	int getBoxWidth();
	int getBoxHeight();

	bool tryFit(int rect_width, int rect_height, int& x_out, int& y_out); // dont forget to call addRectangleIndex / removeRectangleIndex after calling tryFit
	bool tryFit(class RectangleHolder* rectHolder, int boundingBoxIndex);

	// box_width, box_x and box_y refer to the top level bounding box, temperature t
	bool tryFitOverlapping(class RectangleHolder* rectHolder, int rectIdx, int boundingBoxIndex, float t, std::vector<RectangleHolder*>* const rectangles, std::vector<int> indices, int box_width, int box_x, int box_y);
	float calculateOverlappings(std::vector<RectangleHolder*>* rectangles, std::vector<int> indices, int rectIdx, int x, int y, int w, int h, float t, std::vector<int>& transgrIndices);

	void removeLowerLevelBoundingBoxes();
	void addRectangleIndex(int index);
	void removeRectangleIndex(int index);

	int getNumberOfOverlappings();
	void setNumberOfOverlappings(int overlappings);

private:
	//static std::vector<BoundingBox*> solution;
	int rect_width, rect_height;
	int x, y; //global x, y wrt parent bounding box 
	std::shared_ptr<BoundingBox> first;
	std::shared_ptr<BoundingBox> second;
	std::vector<int> rectangleIndices; // only for the top level bounding boxes: indices of all rectangles placed withing this bounding box 
	int numberOfRectangles; // for objective function
	int numberOfOverlappings;
};