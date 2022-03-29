#pragma once
#include "stdafx.h"

class DataHolder {
public:
	DataHolder();
	DataHolder(int rect_amount, int min_rect_width, int min_rect_height, int max_rect_width, int max_rect_height, int boxLength);
	std::shared_ptr<class BoundingBoxCreator> getBoxCreator();
	std::shared_ptr<class RectangleCreator> getRectCreator();
	void OverwriteData(DataHolder* other) ;
	void ResetData();
	void ResetDataForTestEnv();
private:
	std::shared_ptr<class BoundingBoxCreator> boxCreator_;
	std::shared_ptr<class RectangleCreator> rectCreator_;
};