#include "QNeighbourWrapper.h"
#include "NeighbourI.h"
#include "DataHolder.h"
QNeighbourWrapper::QNeighbourWrapper(NeighbourI<DataHolder*>*  n) : neighbour_(n) {

}

void QNeighbourWrapper::Optim() {
	
	neighbour_->optimize();
}

NeighbourI<class DataHolder*>* QNeighbourWrapper::getNeighbourI()
{
	return neighbour_;
}

void QNeighbourWrapper::setNeighbour(NeighbourI<class DataHolder*>* n)
{
	neighbour_ = n;
}
