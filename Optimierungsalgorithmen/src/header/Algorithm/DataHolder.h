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
	void ResetBoundingBoxForTestEnv();
	void ResetRectanglesForTestEnv(const int amount, const int minWidth, const int maxWidth, const int minHeight, const int maxHeight);
	void ResetBoundingBoxContent();
private:
	std::shared_ptr<class BoundingBoxCreator> boxCreator_;
	std::shared_ptr<class RectangleCreator> rectCreator_;
};