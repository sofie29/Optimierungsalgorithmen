#include "BoundingBox.h"

BoundingBox::BoundingBox(int edgeHeight, int edgeWidth)
{
}

void BoundingBox::add(int rect_width, int rect_height)
{
	bool added = false;
	for (BoundingBox* b : solution) {
		if (b->tryFit(rect_width, rect_height)) {
			added = true;
			break;
		}
	}
	if (!added) {
		//BoundingBox* box = new BoundingBox();
	}
}

bool BoundingBox::tryFit(int rect_width, int rect_height)
{
	return false;
}
