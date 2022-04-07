#include "BoundingBox.h"
#include "RectangleHolder.h"


BoundingBox::BoundingBox(int rect_width, int rect_height, int x_pos, int y_pos, RectangleHolder* rectHolder, int rectIndex, int boxIndex) :
	rect_height(rect_height),
	rect_width(rect_width),
	x(x_pos),
	y(y_pos),
	first(nullptr),
	second(nullptr),
	numberOfRectangles(0),
	numberOfOverlappings(0)
{
	this->tryFit(rectHolder, boxIndex);
	this->addRectangleIndex(rectIndex);
}
BoundingBox::BoundingBox(int rect_width, int rect_height, int x, int y) :
	rect_height(rect_height),
	rect_width(rect_width),
	x(x),
	y(y),
	first(nullptr),
	second(nullptr),
	numberOfRectangles(0),
	numberOfOverlappings(0)
{
}


BoundingBox::BoundingBox(int rect_width, int rect_height, int x, int y, std::vector<int> indices, int numberOv = 0) :
	rect_height(rect_height),
	rect_width(rect_width),
	x(x),
	y(y),
	first(nullptr),
	second(nullptr),
	rectangleIndices(indices),
	numberOfRectangles(indices.size()),
	numberOfOverlappings(numberOv)
{
}

// Constructor for OverwriteData method / deep copy
BoundingBox::BoundingBox(int rect_width, int rect_height, int x, int y, std::vector<int> indices, std::shared_ptr<BoundingBox> firstBox, std::shared_ptr<BoundingBox> secondBox, int numberOv) :
	rect_height(rect_height),
	rect_width(rect_width),
	x(x),
	y(y),
	first(firstBox),
	second(secondBox),
	rectangleIndices(indices),
	numberOfRectangles(indices.size()),
	numberOfOverlappings(numberOv)
{
}

BoundingBox::~BoundingBox()
{
	rectangleIndices.clear();
	rectangleIndices.shrink_to_fit();

	removeLowerLevelBoundingBoxes();
}


void BoundingBox::removeLowerLevelBoundingBoxes() {
	first.reset();
	second.reset();
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

bool BoundingBox::tryFitOverlapping(RectangleHolder* rectHolder, int rectIdx, int boundingBoxIndex, float t, std::vector<RectangleHolder*>* const rectangles, std::vector<int> indices, int box_width, int box_x, int box_y)
{
	QRectF& rect = rectHolder->getRectRef();
	int rect_width = rect.width();
	int rect_height = rect.height();
	if (this->first || this->second) { // BoundingBox is not empty

		// try to place rectangle at first BoundingBox
		if (this->first && this->first->tryFitOverlapping(rectHolder, rectIdx, boundingBoxIndex, t, rectangles, indices, box_width, box_x, box_y))
			return true;

		// try to place rectangle at second BoundingBox
		if (this->second && this->second->tryFitOverlapping(rectHolder, rectIdx, boundingBoxIndex, t, rectangles, indices, box_width, box_x, box_y))
			return true;

		return false; // rectangle cannot be placed in neither the first nor the second BoundingBox
	}

	else { // BoundingBox is empty
		if (this->rect_width == 0 || this->rect_height == 0) return false;

		// box_width, box_x and box_y refer to the top level bounding box
		int overlapTopLevelBoxX = this->x - box_x + rect_width - box_width;
		int overlapTopLevelBoxY = this->y - box_y + rect_height - box_width;

		// shift rectangle left or top until it is totally inside the global bounding box
		int rectPosX = overlapTopLevelBoxX > 0 ? this->x - overlapTopLevelBoxX : this->x;
		int	rectPosY = overlapTopLevelBoxY > 0 ? this->y - overlapTopLevelBoxY : this->y;

		std::vector<int> unusedVector;
		float overlapping_area = this->calculateOverlappings(rectangles, indices, rectIdx, rectPosX, rectPosY, rect_width, rect_height, t, unusedVector);
		if (overlapping_area == -1) {
			return false; // overlapping exceeds parameter t
		}
		// all overlappings are smaller than parameter t

		// if rectangle totally fits into the current bounding box, call the simple tryFit method
		if (rect_width <= this->rect_width && rect_height <= this->rect_height) {
			return this->tryFit(rectHolder, boundingBoxIndex); // need this method because of correct child box creation
		}



		// don't place here because rectangle would cover space of another bounding box
		if ((rect_width > this->rect_width && rectPosX == this->x) || (rect_height > this->rect_height && rectPosY == this->y)) return false;
		rect.moveTopLeft(QPointF(rectPosX, rectPosY));
		rectHolder->setBoundingBoxIndex(boundingBoxIndex);
		this->first = rect_width > this->rect_width ?
			std::make_shared<BoundingBox>(0, 0, this->x, this->y) : // position can be chosen arbitrarily
			std::make_shared<BoundingBox>(this->rect_width - rect_width, this->rect_height, this->x + rect_width, this->y);

		this->second = rect_height > this->rect_height ?
			std::make_shared<BoundingBox>(0, 0, this->x, this->y) : // position can be chosen arbitrarily
			std::make_shared<BoundingBox>(this->rect_width, this->rect_height - rect_height, this->x, this->y + rect_height);

		return true;
	}
}

float BoundingBox::calculateOverlappings(std::vector<RectangleHolder*>* rectangles, std::vector<int> indices, int rectIdx, int x, int y, int w, int h, float t, std::vector<int>& transgrIndices)
{
	float overlap_total = 0;
	bool causesTransgression = false;
	for (int i : indices) {
		if (i == rectIdx) {
			continue;
		}

		QRectF& other = (*rectangles)[i]->getRectRef();

		int x_overlap = std::min(x + w, (int)(other.x() + other.width())) - std::max(x, (int)other.x());
		int y_overlap = std::min(y + h, (int)(other.y() + other.height())) - std::max(y, (int)other.y());

		int area_overlap = (x_overlap > 0 && y_overlap > 0) ? x_overlap * y_overlap : 0;

		int area_1 = w * h;
		int area_2 = other.width() * other.height();
		int area_max = std::max(area_1, area_2);

		float overlap = (float)area_overlap / (float)area_max;

		if (overlap > t) {
			transgrIndices.push_back(i);
			causesTransgression = true;
		}

		overlap_total += area_overlap;

	}
	return causesTransgression ? -1 : overlap_total;
}

int BoundingBox::getXPos() const
{
	return x;
}

int BoundingBox::getYPos() const
{
	return y;
}

std::shared_ptr<BoundingBox> BoundingBox::getFirstBox()
{
	return first;
}

std::shared_ptr<BoundingBox> BoundingBox::getSecondBox()
{
	return second;
}

std::vector<int> BoundingBox::getRectangleIndices() {
	return rectangleIndices;
}

int BoundingBox::getBoxWidth()
{
	return rect_width;
}

int BoundingBox::getBoxHeight()
{
	return rect_height;
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
}

int BoundingBox::getNumberOfOverlappings()
{
	return numberOfOverlappings;
}

void BoundingBox::setNumberOfOverlappings(int overlappings)
{
	numberOfOverlappings = overlappings;
}
