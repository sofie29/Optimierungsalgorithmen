#pragma once
#include "stdafx.h"
class RectangleDrawer : public QWidget {
	Q_OBJECT
public:
	RectangleDrawer();
	~RectangleDrawer();
	void DrawOnScreen(QGraphicsScene* scene, bool drawOld);
	void getRects(std::vector<QRectF>& list);
	void setRectsQRectF(std::vector<QRectF>& list);
signals:
	void EmitRectInformation(const int amount, const int minWidth, const int maxWidth, const int minHeight, const int maxHeight);
	void EmitListChanged();
public slots:
	void DrawRectAmountChangedI(const int amount);
	void DrawRectAmountChangedS(const QString& amount);

	void DrawRectMinWidthChangedI(const int minWidth);
	void DrawRectMinWidthChangedS(const QString& minWidth);
	void DrawRectMaxWidthChangedI(const int maxWidth);
	void DrawRectMaxWidthChangedS(const QString& maxWidth);
	void DrawRectMinHeightChangedI(const int minHeight);
	void DrawRectMinHeightChangedS(const QString& minHeight);
	void DrawRectMaxHeightChangedI(const int maxHeight);
	void DrawRectMaxHeightChangedS(const QString& maxHeight);

	void SetRects(const std::vector<class RectangleHolder*>* list);
	void OnBoundingBoxSizeChanged(int x);
private:
	void SetOldRects();

	int currentAmount_;
	int currentMinWidth_;
	int currentMaxWidth_;
	int currentMinHeight_;
	int currentMaxHeight_;
	std::vector<QRectF> rectList_;
	std::vector<QRectF> rectListOld_;
	std::vector<QColor> rectColors_;
	
};