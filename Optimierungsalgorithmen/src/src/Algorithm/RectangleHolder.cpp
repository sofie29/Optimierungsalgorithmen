#include "RectangleHolder.h"

RectangleHolder::RectangleHolder(QRectF rect) : rect_(rect)
{
	drawColor_ = defaultColor_;
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

QColor RectangleHolder::getColor()
{
	return drawColor_;
}

void RectangleHolder::setColor(QColor c)
{
	drawColor_ = c;
}

void RectangleHolder::setToSwappedColor()
{
	drawColor_ = swappedColor_;
}

void RectangleHolder::setToDefaultColor()
{
	drawColor_ = defaultColor_;
}

void RectangleHolder::rotateRect()
{
	int width = rect_.width();
	this->rect_.setWidth(rect_.height());
	this->rect_.setHeight(width);
}




QColor RectangleHolder::defaultColor_ = QColor(0.0, 55.0, 55.0, 140.0);
QColor RectangleHolder::swappedColor_ = QColor(224.0, 181.0, 36.0, 140.0);