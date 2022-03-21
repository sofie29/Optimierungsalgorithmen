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
