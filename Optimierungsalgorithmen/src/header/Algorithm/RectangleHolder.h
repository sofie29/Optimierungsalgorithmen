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
	QColor getColor();
	void setColor(QColor c);
	void setToSwappedColor();
	void setToDefaultColor();
	void rotateRect();
private:
	QRectF rect_;
	int boundingBoxIndex_;
	QColor drawColor_;

	static QColor defaultColor_;
	static QColor swappedColor_;
};