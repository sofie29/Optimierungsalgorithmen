#pragma once
#include "stdafx.h"

class Drawer : public QWidget {
	Q_OBJECT
public:
	Drawer(QGraphicsScene* scene);
	~Drawer();
	class RectangleDrawer* getRectDrawer();
	class BoundingBoxDrawer* getBBoxDrawer();

public slots:
	void DrawScene();
	void DrawSwappedRects();
private:
	QGraphicsScene* scene_;
	class RectangleDrawer* rectDrawer_;
	class BoundingBoxDrawer* boundingBoxDrawer_;
	

};