#pragma once
#include "stdafx.h"

class BoundingBoxDrawer : public QWidget {
	Q_OBJECT
public:
	BoundingBoxDrawer();
	~BoundingBoxDrawer();
	void DrawOnScene(QGraphicsScene* scene, bool drawOld);
	void getBoundingBoxList(std::vector<QRectF>& list);
signals:
	void BoundingBoxSizeChanged(const int edgeLength);
	void EmitListChanged();
public slots:
	void BoundingBoxSizeChangedS(const QString& maxEdgeLength);
	void BoundingBoxSizeChangedI(const int maxEdgeLength);
	void SetBoundingBoxes(const std::vector<QRectF>& list);
private:
	void SetOldBoundingBoxes();
	int currentBoxLength_;
	std::vector<QRectF> boundingBoxList_;
	std::vector<QRectF> boundingBoxListOld_;
};