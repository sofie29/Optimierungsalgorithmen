#pragma once
#include "stdafx.h"

class MainScene : public QGraphicsScene {
	Q_OBJECT
public:
	MainScene(QMainWindow* win);
	~MainScene();
	class RectangleDrawer* getRecDrawer() const;
	class BoundingBoxDrawer* getBoxDrawer() const;


	class Drawer* getDrawer() const;
protected:
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
signals:
	void Clicked(QPointF p);
public slots:
	void DrawRect(QPointF p);
private:
	class Drawer* drawer_;
};