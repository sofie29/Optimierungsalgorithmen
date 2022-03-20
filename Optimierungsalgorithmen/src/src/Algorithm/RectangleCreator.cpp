#include "RectangleCreator.h"

RectangleCreator::RectangleCreator()
{
}
void RectangleCreator::getRectList(std::vector<QRectF>& list)
{
	list = rectangleList_;
}

void RectangleCreator::setNewRectList(std::vector<QRectF>& list)
{
	rectangleList_.clear();
	for (const auto& rect : list) {
		rectangleList_.emplace_back(QRectF(rect.x(), rect.y(), rect.width(), rect.height()));
	}
}

void RectangleCreator::setRectList(std::vector<QRectF>& list)
{
	rectangleList_ = list;
}



void RectangleCreator::CreateRects(const int amount, const int maxEdgeLength) {

	rectangleList_.clear();

	float squareSize = 0.0f;

	std::random_device rd;
	std::mt19937 engine(rd());
	std::uniform_int_distribution<> dist(AlgorithmConstants::minRectangleEdgeSize_, maxEdgeLength);
	int recsPerLine = std::ceil(std::sqrt(amount));
	for (int i = 0; i < amount; i++) {

		int height = dist(engine);
		int width = dist(engine);
		//int x_pos = (i % recsPerLine) * (AlgorithmConstants::maxBoxEdgeSize_ + UIConstants::rectangleSpace_);
		//int y_pos = (int)(i / (float)recsPerLine) * (AlgorithmConstants::maxBoxEdgeSize_ + UIConstants::rectangleSpace_);
		int x_pos = 0;
		int y_pos = 0;
		QRectF rect(x_pos, y_pos, width, height);
		rectangleList_.emplace_back(rect);
		squareSize += height * width;
	}
	emit RectListUpdated(rectangleList_);
	emit EmitSquareSize(squareSize);
}
void RectangleCreator::OnOptimDone() {
	emit EmitRectList(rectangleList_);
}