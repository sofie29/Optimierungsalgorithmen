#include "RectangleDrawer.h"

RectangleDrawer::RectangleDrawer(QGraphicsScene* scene) : scene_(scene)
{
	currentAmount_ = AlgorithmConstants::initialAmount_;
	currentMaxEdgeLength_ = AlgorithmConstants::initialEdgeSize_;
}


RectangleDrawer::~RectangleDrawer()
{
	scene_ = nullptr;
}

void RectangleDrawer::DrawRectAmountChangedI(const int amount)
{
	if (amount <= AlgorithmConstants::maxRectangleAmount_) {
		currentAmount_ = amount;
		DrawRects();
	}
}


void RectangleDrawer::DrawRectAmountChangedS(const QString& amount)
{
	if (amount.toInt() <= AlgorithmConstants::maxRectangleAmount_) {
		currentAmount_ = amount.toInt();
		DrawRects();
	}
}

void RectangleDrawer::DrawRectSizeChangedI(const int maxEdgeLength)
{
	if (maxEdgeLength <= AlgorithmConstants::maxRectangleEdgeSize_ && maxEdgeLength >= AlgorithmConstants::minRectangleEdgeSize_) {
		currentMaxEdgeLength_ = maxEdgeLength;
		DrawRects();
	}
}

void RectangleDrawer::DrawRectSizeChangedS(const QString& maxEdgeLength)
{
	if (maxEdgeLength.toInt() <= AlgorithmConstants::maxRectangleEdgeSize_ && maxEdgeLength.toInt() >= AlgorithmConstants::minRectangleEdgeSize_){
		currentMaxEdgeLength_ = maxEdgeLength.toInt();
		DrawRects();
	}
}

void RectangleDrawer::DrawRects() {
	scene_->clear();
	std::random_device rd;
	std::mt19937 engine(rd());
	std::uniform_int_distribution<> dist(AlgorithmConstants::minRectangleEdgeSize_, currentMaxEdgeLength_);

	//int maxLines = (int)(UIConstants::mainSceneHeight_ / (float)(maxEdgeLength + UIConstants::rectangleSpace_));
	//int pixelForNewLine = UIConstants::mainWSceneWidth_ /
	int recsPerLine =  std::ceil(std::sqrt(currentAmount_));
	for (int i = 0; i < currentAmount_; i++) {
		
		int height =  dist(engine);
		int width = dist(engine);
		int x_pos = (i % recsPerLine) * (currentMaxEdgeLength_ + UIConstants::rectangleSpace_);
		int y_pos = (int)(i / (float)recsPerLine) * (currentMaxEdgeLength_ + UIConstants::rectangleSpace_);
		QRectF rect(x_pos, y_pos, width, height);
		scene_->addRect(rect);
	}
	scene_->update();
}