#include "RectangleDrawer.h"
#include "RectangleHolder.h"
RectangleDrawer::RectangleDrawer() 
{
	currentAmount_ = AlgorithmConstants::initialAmount_;
	
	currentMinWidth_ = AlgorithmConstants::initialEdgeSize_;
	currentMaxWidth_ = AlgorithmConstants::initialEdgeSize_;
	currentMinHeight_ = AlgorithmConstants::initialEdgeSize_;
	currentMaxHeight_ = AlgorithmConstants::initialEdgeSize_;
}


RectangleDrawer::~RectangleDrawer()
{
	rectList_.clear();
	rectList_.shrink_to_fit();

	rectListOld_.clear();
	rectListOld_.shrink_to_fit();

	rectColors_.clear();
	rectColors_.shrink_to_fit();
}

void RectangleDrawer::setRectsQRectF(std::vector<QRectF>& list) {
	rectList_ = list;
}

void RectangleDrawer::DrawOnScreen(QGraphicsScene* scene, bool drawOld)
{
	QBrush brush;
	brush.setStyle(Qt::BrushStyle::SolidPattern);
	brush.setColor(QColor(0, 139, 139, 255));
	QPen pen;
	pen.setColor(Qt::black);
	pen.setWidth(1);
	int idx = 0;
	if (drawOld) {
		for (const QRectF& rect : rectListOld_) {
			brush.setColor(rectColors_[idx]);
			scene->addRect(rect, pen, brush);
			++idx;
		}
	}
	else {
		for (const QRectF& rect : rectList_) {
			brush.setColor(rectColors_[idx]);
			scene->addRect(rect, pen, brush);
			++idx;
		}
	}
}

void RectangleDrawer::getRects(std::vector<QRectF>& list)
{
	list = rectList_;
}

void RectangleDrawer::DrawRectAmountChangedI(const int amount)
{
	if (amount <= AlgorithmConstants::maxRectangleAmount_) {
		currentAmount_ = amount;
		emit EmitRectInformation(currentAmount_, currentMinWidth_, currentMaxWidth_, currentMinHeight_, currentMaxHeight_);
	}
}


void RectangleDrawer::DrawRectAmountChangedS(const QString& amount)
{
	if (amount.toInt() <= AlgorithmConstants::maxRectangleAmount_) {
		currentAmount_ = amount.toInt();
		emit EmitRectInformation(currentAmount_, currentMinWidth_, currentMaxWidth_, currentMinHeight_, currentMaxHeight_);
	}
}

void RectangleDrawer::DrawRectMinWidthChangedI(const int minWidth)
{
	if (minWidth <= AlgorithmConstants::maxRectangleEdgeSize_ && minWidth >= AlgorithmConstants::minRectangleEdgeSize_) {
		currentMinWidth_ = minWidth;
		emit EmitRectInformation(currentAmount_, currentMinWidth_, currentMaxWidth_, currentMinHeight_, currentMaxHeight_);
	}
}

void RectangleDrawer::DrawRectMinWidthChangedS(const QString& minWidth)
{
	if (minWidth.toInt() <= AlgorithmConstants::maxRectangleEdgeSize_ && minWidth.toInt() >= AlgorithmConstants::minRectangleEdgeSize_) {
		currentMinWidth_ = minWidth.toInt();
		emit EmitRectInformation(currentAmount_, currentMinWidth_, currentMaxWidth_, currentMinHeight_, currentMaxHeight_);
	}
}

void RectangleDrawer::DrawRectMaxWidthChangedI(const int maxWidth)
{
	if (maxWidth <= AlgorithmConstants::maxRectangleEdgeSize_ && maxWidth >= AlgorithmConstants::minRectangleEdgeSize_) {
		currentMaxWidth_ = maxWidth;
		emit EmitRectInformation(currentAmount_, currentMinWidth_, currentMaxWidth_, currentMinHeight_, currentMaxHeight_);
	}
}

void RectangleDrawer::DrawRectMaxWidthChangedS(const QString& maxWidth)
{
	if (maxWidth.toInt() <= AlgorithmConstants::maxRectangleEdgeSize_ && maxWidth.toInt() >= AlgorithmConstants::minRectangleEdgeSize_) {
		currentMaxWidth_ = maxWidth.toInt();
		emit EmitRectInformation(currentAmount_, currentMinWidth_, currentMaxWidth_, currentMinHeight_, currentMaxHeight_);
	}
}

void RectangleDrawer::DrawRectMinHeightChangedI(const int minHeight)
{
	if (minHeight <= AlgorithmConstants::maxRectangleEdgeSize_ && minHeight >= AlgorithmConstants::minRectangleEdgeSize_) {
		currentMinHeight_ = minHeight;
		emit EmitRectInformation(currentAmount_, currentMinWidth_, currentMaxWidth_, currentMinHeight_, currentMaxHeight_);
	}
}

void RectangleDrawer::DrawRectMinHeightChangedS(const QString& minHeight)
{
	if (minHeight.toInt() <= AlgorithmConstants::maxRectangleEdgeSize_ && minHeight.toInt() >= AlgorithmConstants::minRectangleEdgeSize_) {
		currentMinHeight_ = minHeight.toInt();
		emit EmitRectInformation(currentAmount_, currentMinWidth_, currentMaxWidth_, currentMinHeight_, currentMaxHeight_);
	}
}

void RectangleDrawer::DrawRectMaxHeightChangedI(const int maxHeight)
{
	if (maxHeight <= AlgorithmConstants::maxRectangleEdgeSize_ && maxHeight >= AlgorithmConstants::minRectangleEdgeSize_) {
		currentMaxHeight_ = maxHeight;
		emit EmitRectInformation(currentAmount_, currentMinWidth_, currentMaxWidth_, currentMinHeight_, currentMaxHeight_);
	}
}

void RectangleDrawer::DrawRectMaxHeightChangedS(const QString& maxHeight)
{
	if (maxHeight.toInt() <= AlgorithmConstants::maxRectangleEdgeSize_ && maxHeight.toInt() >= AlgorithmConstants::minRectangleEdgeSize_) {
		currentMaxHeight_ = maxHeight.toInt();
		emit EmitRectInformation(currentAmount_, currentMinWidth_, currentMaxWidth_, currentMinHeight_, currentMaxHeight_);
	}
}


void RectangleDrawer::SetRects(const std::vector<class RectangleHolder*>* list) {
	
	SetOldRects();
	rectList_.clear();
	rectColors_.clear();
	for (class RectangleHolder* r : *list) {
		rectList_.emplace_back(r->getRect());
		rectColors_.emplace_back(r->getColor());		
		
	}
	//emit EmitListChanged();

	/*
	scene_->clear();
	
	for (const QRectF& rect : list) {
		
		
		scene_->addRect(rect);
	}
	scene_->update();
	*/
}

void RectangleDrawer::OnBoundingBoxSizeChanged(int x) {
	emit EmitRectInformation(currentAmount_, currentMinWidth_, currentMaxWidth_, currentMinHeight_, currentMaxHeight_);
}

void RectangleDrawer::SetOldRects()
{
	rectListOld_.clear();
	for (QRectF rect : rectList_) {
		rectListOld_.emplace_back(rect);
	}
}
