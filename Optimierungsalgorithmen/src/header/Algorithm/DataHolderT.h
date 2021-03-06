#pragma once
#include "stdafx.h"
#include "DataHolder.h"

template<class Data>
class DataHolderT {
public:
	DataHolderT(Data dt);
	~DataHolderT();
	Data getData();
	void OverwriteData(DataHolderT<Data>* other);
	void ResetData();
	void SoftResetData();
private:
	Data data_;
};

template<class Data>
inline DataHolderT<Data>::DataHolderT(Data dt)
{
	data_ = dt;
}

template<class Data>
inline DataHolderT<Data>::~DataHolderT()
{
}

template<class Data>
inline Data DataHolderT<Data>::getData()
{
	return data_;
}

template<class Data>
inline void DataHolderT<Data>::OverwriteData(DataHolderT<Data>* other)
{
}

template<class Data>
inline void DataHolderT<Data>::ResetData()
{
}

template<>
inline void DataHolderT<DataHolder*>::SoftResetData()
{
	data_->ResetBoundingBoxContent();
}

template<>
inline void DataHolderT<DataHolder*>::OverwriteData(DataHolderT<DataHolder*>* other) {
	data_->OverwriteData(other->getData());
}

template<>
inline void DataHolderT<DataHolder*>::ResetData() {
	if (UIConstants::useUI_)
		data_->ResetData();
	else data_->ResetBoundingBoxForTestEnv();
}