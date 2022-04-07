#include "RectangleCreator.h"
#include "RectangleHolder.h"
RectangleCreator::RectangleCreator()
{
	rectangleList_ = new std::vector<class RectangleHolder*>();
	squareSize_ = 0.0f;

	amount_ = 0;
	minWidth_ = 0;
	maxWidth_ = 0;
	minHeight_ = 0;
	maxHeight_ = 0;
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
	int idx = 0;
	for (class RectangleHolder* r : *list) {
		QRectF rect = r->getRect();
		rectangleList_->emplace_back(new class RectangleHolder(rect, r->getBoundingBoxIndex()));
		(*rectangleList_)[idx]->setColor(r->getColor());
		++idx;
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

void RectangleCreator::ResetRectColors()
{
	for (RectangleHolder* r : *rectangleList_) {
		r->setToDefaultColor();
	}
}

float RectangleCreator::getSquareSize()
{
	return squareSize_;
}

void RectangleCreator::ResetRectsForTestEnv(const int amount, const int minWidth, const int maxWidth, const int minHeight, const int maxHeight)
{
	for (RectangleHolder* r : *rectangleList_) {
		delete r;
		r = nullptr;
	}
	rectangleList_->clear();
	// rectangleList_->shrink_to_fit();

	
	squareSize_ = 0.0f;

	std::random_device rd;
	std::mt19937 engine(rd());
	std::uniform_int_distribution<> dist_width(minWidth, maxWidth);
	std::uniform_int_distribution<> dist_height(minHeight, maxHeight);


	int recsPerLine = std::ceil(std::sqrt(amount));
	for (int i = 0; i < amount; i++) {

		int height = dist_height(engine);
		int width = dist_width(engine);
		int x_pos = 0;
		int y_pos = 0;
		QRectF rect(x_pos, y_pos, width, height);
		rectangleList_->emplace_back(new class RectangleHolder(rect));
		squareSize_ += height * width;
	}
}

void RectangleCreator::ShuffleList()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(rectangleList_->begin(), rectangleList_->end(), std::default_random_engine(seed));
}


void RectangleCreator::CreateRects(const int amount, const int minWidth, const int maxWidth, const int minHeight, const int maxHeight)
{
	for (RectangleHolder* r : *rectangleList_) {
		delete r;
		r = nullptr;
	}
	rectangleList_->clear();
	rectangleList_->shrink_to_fit();

	amount_ = amount;
	minWidth_ = minWidth;
	maxWidth_ = maxWidth;
	minHeight_ = minHeight;
	maxHeight_ = maxHeight;
	squareSize_ = 0.0f;

	std::random_device rd;
	std::mt19937 engine(rd());
	std::uniform_int_distribution<> dist_width(minWidth_, maxWidth_);
	std::uniform_int_distribution<> dist_height(minHeight_, maxHeight_);


	int recsPerLine = std::ceil(std::sqrt(amount_));
	for (int i = 0; i < amount_; i++) {

		int height = dist_height(engine);
		int width = dist_width(engine);
		int x_pos = 0;
		int y_pos = 0;
		QRectF rect(x_pos, y_pos, width, height);
		rectangleList_->emplace_back(new class RectangleHolder(rect));
		squareSize_ += height * width;
	}

	if (UIConstants::useUI_) {
		emit RectListUpdated(rectangleList_);
		emit EmitSquareSize(squareSize_);
	}
}
void RectangleCreator::OnOptimDone() {
	emit EmitRectList(rectangleList_);
}