#include "RectangleCreator.h"
#include "RectangleHolder.h"
RectangleCreator::RectangleCreator()
{
	rectangleList_ = new std::vector<class RectangleHolder*>();
}
RectangleCreator::~RectangleCreator()
{
	for (RectangleHolder* r : *rectangleList_) {
		delete r;
		r = nullptr;
	}
	rectangleList_->clear();
	rectangleList_->shrink_to_fit();
}
std::vector<class RectangleHolder*>* RectangleCreator::getRectList()
{
	return rectangleList_;
}

void RectangleCreator::setNewRectList(std::vector<class RectangleHolder*>* list)
{
	for (RectangleHolder* r : *rectangleList_) {
		delete r;
		r = nullptr;
	}
	rectangleList_->clear();
	rectangleList_->shrink_to_fit();
	for (class RectangleHolder* r : *list) {
		QRectF rect = r->getRect();
		rectangleList_->emplace_back(new class RectangleHolder(rect, r->getBoundingBoxIndex()));
		// rectangleList_->emplace_back(new class RectangleHolder(rect));
	}
}

void RectangleCreator::setRectList(std::vector<class RectangleHolder*>* list)
{
	rectangleList_ = list;
}

void RectangleCreator::ResetData()
{
	for (RectangleHolder* r : *rectangleList_) {
		delete r;
		r = nullptr;
	}
	rectangleList_->clear();
	rectangleList_->shrink_to_fit();
}



void RectangleCreator::CreateRects(const int amount, const int maxEdgeLength) {

	for (RectangleHolder* r : *rectangleList_) {
		delete r;
		r = nullptr;
	}
	rectangleList_->clear();
	rectangleList_->shrink_to_fit();

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
		rectangleList_->emplace_back(new class RectangleHolder(rect));
		squareSize += height * width;
	}
	emit RectListUpdated(rectangleList_);
	emit EmitSquareSize(squareSize);
}
void RectangleCreator::OnOptimDone() {
	emit EmitRectList(rectangleList_);
}