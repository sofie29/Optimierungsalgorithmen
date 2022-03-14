#include "RectangleCreator.h"

RectangleCreator::RectangleCreator()
{
}
void RectangleCreator::getRectList(std::vector<QRectF>& list)
{
	list = rectangleList;
}

void RectangleCreator::OnRectPositionsUpdated(const std::vector<QRectF>& list)
{
	rectangleList = list;
	emit RectListUpdated(rectangleList);
}

void RectangleCreator::CreateRects(const int amount, const int maxEdgeLength) {

	rectangleList.clear();

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
		rectangleList.emplace_back(rect);
	}
	emit RectListCreated();
}