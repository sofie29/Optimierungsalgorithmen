#pragma once
#include "stdafx.h"

class DataHolder {
public:
	DataHolder();
	
	std::shared_ptr<class BoundingBoxCreator> getBoxCreator();
	std::shared_ptr<class RectangleCreator> getRectCreator();
	void OverwriteData(DataHolder* other) ;
private:
	std::shared_ptr<class BoundingBoxCreator> boxCreator_;
	std::shared_ptr<class RectangleCreator> rectCreator_;
};