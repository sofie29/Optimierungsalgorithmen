#include "RectangleHolder.h"

RectangleHolder::RectangleHolder(QRectF rect) : rect_(rect)
{
}

RectangleHolder::RectangleHolder(QRectF rect, int idx) : rect_(rect), boundingBoxIndex_(idx)
{
}

RectangleHolder::~RectangleHolder()
{
}

QRectF RectangleHolder::getRect()
{
	return rect_;
}

QRectF& RectangleHolder::getRectRef()
{
	return rect_;
	// TODO: insert return statement here
}

void RectangleHolder::setRect(QRectF rect)
{
	rect_ = rect;
}

int RectangleHolder::getBoundingBoxIndex()
{
	return boundingBoxIndex_;
}

void RectangleHolder::setBoundingBoxIndex(int idx)
{
	boundingBoxIndex_ = idx;
}
