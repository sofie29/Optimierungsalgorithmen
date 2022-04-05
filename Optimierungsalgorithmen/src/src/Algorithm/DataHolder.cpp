#include "DataHolder.h"
#include "RectangleCreator.h"
#include "BoundingBoxCreator.h"
#include "BoundingBox.h"
#include "RectangleHolder.h"
DataHolder::DataHolder() 
{
	boxCreator_ = std::make_shared<BoundingBoxCreator>();
	rectCreator_ = std::make_shared<RectangleCreator>();
}

DataHolder::DataHolder(int rect_amount, int min_rect_width, int min_rect_height, int max_rect_width, int max_rect_height, int boxLength)
{
	boxCreator_ = std::make_shared<BoundingBoxCreator>(boxLength);
	rectCreator_ = std::make_shared<RectangleCreator>();
	rectCreator_->CreateRects(rect_amount, min_rect_width, min_rect_height, max_rect_width, max_rect_height);
}



std::shared_ptr<class BoundingBoxCreator> DataHolder::getBoxCreator()
{
	return boxCreator_;
}

std::shared_ptr<class RectangleCreator> DataHolder::getRectCreator()
{
	return rectCreator_;
}

void DataHolder::OverwriteData(DataHolder* other)
{

	

	std::vector<std::shared_ptr<BoundingBox>> bBoxList;
	other->getBoxCreator()->getBoundingBoxList(bBoxList);
	boxCreator_->setBoundingBoxList(bBoxList);



	std::vector<QRectF> bbRectList;
	other->getBoxCreator()->getRectList(bbRectList);
	boxCreator_->setRectList(bbRectList);

	boxCreator_->setCurrentFitScore(other->getBoxCreator()->getCurrentFitScore());


	std::vector<RectangleHolder*>* rectList = other->getRectCreator()->getRectList();
	rectCreator_->setNewRectList(rectList);

	
}

void DataHolder::ResetData()
{
	boxCreator_->ResetBoundingBoxList();
	rectCreator_->ResetRectColors();
}

void DataHolder::ResetBoundingBoxForTestEnv()
{
	boxCreator_->ResetBoundingBoxList();
	boxCreator_->RecSquareSizeChanged(rectCreator_->getSquareSize());
}

void DataHolder::ResetRectanglesForTestEnv(const int amount, const int minWidth, const int maxWidth, const int minHeight, const int maxHeight)
{
	rectCreator_->ResetRectsForTestEnv(amount, minWidth, maxWidth, minHeight, maxHeight);
}

void DataHolder::ResetBoundingBoxContent()
{
	boxCreator_->ResetBoundingBoxContent();
}
