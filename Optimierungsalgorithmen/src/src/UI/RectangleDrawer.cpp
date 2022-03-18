#include "RectangleDrawer.h"

RectangleDrawer::RectangleDrawer() 
{
	currentAmount_ = AlgorithmConstants::initialAmount_;
	currentMaxEdgeLength_ = AlgorithmConstants::initialEdgeSize_;

	
}


RectangleDrawer::~RectangleDrawer()
{
	
}

void RectangleDrawer::DrawOnScreen(QGraphicsScene* scene)
{
	QBrush brush;
	brush.setStyle(Qt::BrushStyle::SolidPattern);
	brush.setColor(Qt::darkCyan);
	QPen pen;
	pen.setColor(Qt::black);
	pen.setWidth(1);
	for (const QRectF& rect : rectList_) {
		scene->addRect(rect, pen, brush);
	}
}

void RectangleDrawer::DrawRectAmountChangedI(const int amount)
{
	if (amount <= AlgorithmConstants::maxRectangleAmount_) {
		currentAmount_ = amount;
		emit EmitRectInformation(currentAmount_, currentMaxEdgeLength_);
	}
}


void RectangleDrawer::DrawRectAmountChangedS(const QString& amount)
{
	if (amount.toInt() <= AlgorithmConstants::maxRectangleAmount_) {
		currentAmount_ = amount.toInt();
		emit EmitRectInformation(currentAmount_, currentMaxEdgeLength_);
	}
}

void RectangleDrawer::DrawRectSizeChangedI(const int maxEdgeLength)
{
	if (maxEdgeLength <= AlgorithmConstants::maxRectangleEdgeSize_ && maxEdgeLength >= AlgorithmConstants::minRectangleEdgeSize_) {
		currentMaxEdgeLength_ = maxEdgeLength;
		emit EmitRectInformation(currentAmount_, currentMaxEdgeLength_);
	}
}

void RectangleDrawer::DrawRectSizeChangedS(const QString& maxEdgeLength)
{
	if (maxEdgeLength.toInt() <= AlgorithmConstants::maxRectangleEdgeSize_ && maxEdgeLength.toInt() >= AlgorithmConstants::minRectangleEdgeSize_){
		currentMaxEdgeLength_ = maxEdgeLength.toInt();
		emit EmitRectInformation(currentAmount_, currentMaxEdgeLength_);
	}
}

void RectangleDrawer::SetRects(const std::vector<QRectF>& list) {
	
	rectList_ = list;
	emit EmitListChanged();

	/*
	scene_->clear();
	
	for (const QRectF& rect : list) {
		
		
		scene_->addRect(rect);
	}
	scene_->update();
	*/
}

void RectangleDrawer::OnBoundingBoxSizeChanged(int x) {
	emit EmitRectInformation(currentAmount_, currentMaxEdgeLength_);
}
