#include "DataHolder.h"
#include "RectangleCreator.h"
#include "BoundingBoxCreator.h"
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
