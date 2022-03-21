#pragma once
#include "stdafx.h"

class RectangleHolder {
public:
	RectangleHolder(QRectF rect);
	RectangleHolder(QRectF rect, int idx);
	~RectangleHolder();
	QRectF getRect();
	QRectF& getRectRef();
	void setRect(QRectF rect);
	int getBoundingBoxIndex();
	void setBoundingBoxIndex(int idx);
private:
	QRectF rect_;
	int boundingBoxIndex_;
};