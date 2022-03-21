﻿#include "BoundingBox.h"
#include "RectangleHolder.h"

BoundingBox::BoundingBox(int rect_width, int rect_height, int x_pos, int y_pos, QRectF& rect, int rectIndex) :
	rect_height(rect_height),
	rect_width(rect_width),
	x(x_pos),
	y(y_pos),
	first(nullptr),
	second(nullptr),
	numberOfRectangles(0)
{
	int x, y;
	this->tryFit(rect.width(), rect.height(), x, y);
	this->addRectangleIndex(rectIndex);
	rect.moveTopLeft(QPointF(x, y));
	
}
BoundingBox::BoundingBox(int rect_width, int rect_height, int x, int y):
rect_height(rect_height),
rect_width(rect_width),
x(x),
y(y),
first(nullptr),
second(nullptr),
numberOfRectangles(0)
{
}
/*
void BoundingBox::add(int rect_width, int rect_height)
{
	
	BoundingBox test(10, 10, 0, 0);
	bool fit = test.tryFit(rect_width, rect_height);
	std::cout << fit << std::endl;
	bool added = false;


	for (BoundingBox* box : solution) {
		
		if (box->tryFit(rect_width, rect_height)) {
			added = true;
			break;
		}
		
	}
	if (!added) {
		//BoundingBox* box = new BoundingBox();
	}

}
*/

void BoundingBox::removeLowerLevelBoundingBoxes() {
	first.reset();
	second.reset();
	// number of rectangles = 0 ?
}

bool BoundingBox::tryFit(int rect_width, int rect_height, int& x_out, int& y_out)
{
	if (this->first || this->second) { // BoundingBox is not empty

		// try to place rectangle at first BoundingBox
		if (this->first && this->first->tryFit(rect_width, rect_height, x_out, y_out))
			return true;

		// try to place rectangle at second BoundingBox
		if (this->second && this->second->tryFit(rect_width, rect_height, x_out, y_out))
			return true;

		return false; // rectangle cannot be placed in neither the first nor the second BoundingBox
	}

	else { // BoundingBox is empty

		int emptySpaceX = this->rect_width - rect_width;
		int emptySpaceY = this->rect_height - rect_height;

		if (emptySpaceX < 0 || emptySpaceY < 0)
			return false; // rectangle does not fit

		else {

			// place rectangle in this BoundingBox

			if (emptySpaceX < emptySpaceY) {
				// split space along x axis
				this->first = std::shared_ptr<BoundingBox>(new BoundingBox(emptySpaceX, rect_height, this->x + rect_width, this->y));
				this->second = std::shared_ptr<BoundingBox>(new BoundingBox(this->rect_width, emptySpaceY, this->x, this->y + rect_height));
			}
			else {
				// split space along y axis
				this->first = std::shared_ptr<BoundingBox>(new BoundingBox(rect_width, emptySpaceY, this->x, this->y + rect_height));
				this->second = std::shared_ptr<BoundingBox>(new BoundingBox(emptySpaceX, this->rect_height, this->x + rect_width, this->y));
			}

			// set rectangle position
			x_out = this->x;
			y_out = this->y;

			return true;
		}
	}
	return false;

}

bool BoundingBox::tryFit(RectangleHolder* rectHolder, int boundingBoxIndex)
{
	QRectF& rect = rectHolder->getRectRef();
	int rect_width = rect.width();
	int rect_height = rect.height();
	if (this->first || this->second) { // BoundingBox is not empty

	// try to place rectangle at first BoundingBox
		if (this->first && this->first->tryFit(rectHolder, boundingBoxIndex))
			return true;

		// try to place rectangle at second BoundingBox
		if (this->second && this->second->tryFit(rectHolder, boundingBoxIndex))
			return true;

		return false; // rectangle cannot be placed in neither the first nor the second BoundingBox
	}

	else { // BoundingBox is empty

		int emptySpaceX = this->rect_width - rect_width;
		int emptySpaceY = this->rect_height - rect_height;

		if (emptySpaceX < 0 || emptySpaceY < 0)
			return false; // rectangle does not fit

		else {

			// place rectangle in this BoundingBox

			if (emptySpaceX < emptySpaceY) {
				// split space along x axis
				this->first = std::shared_ptr<BoundingBox>(new BoundingBox(emptySpaceX, rect_height, this->x + rect_width, this->y));
				this->second = std::shared_ptr<BoundingBox>(new BoundingBox(this->rect_width, emptySpaceY, this->x, this->y + rect_height));
			}
			else {
				// split space along y axis
				this->first = std::shared_ptr<BoundingBox>(new BoundingBox(rect_width, emptySpaceY, this->x, this->y + rect_height));
				this->second = std::shared_ptr<BoundingBox>(new BoundingBox(emptySpaceX, this->rect_height, this->x + rect_width, this->y));
			}

			// set rectangle position
			rect.moveTopLeft(QPointF(this->x, this->y));
			rectHolder->setBoundingBoxIndex(boundingBoxIndex);
			return true;
		}
	}
	return false;
}

int BoundingBox::getXPos() const
{
	return x;
}

int BoundingBox::getYPos() const
{
	return y;
}

std::vector<int> BoundingBox::getRectangleIndices() {
	return rectangleIndices;
}

void BoundingBox::addRectangleIndex(int index) {
	rectangleIndices.push_back(index);
	++numberOfRectangles;
	return;
}


void BoundingBox::removeRectangleIndex(int index) {
	rectangleIndices.erase(std::remove(rectangleIndices.begin(), rectangleIndices.end(), index), rectangleIndices.end());
	--numberOfRectangles;
	
	return;

	// TODO: if (--numberOfRectangles == 0) delete this bounding box
}
