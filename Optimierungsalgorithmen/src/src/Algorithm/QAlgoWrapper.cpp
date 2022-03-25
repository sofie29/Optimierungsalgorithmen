#include "QAlgoWrapper.h"

QAlgoWrapper::QAlgoWrapper(class OptimAlgoI<class DataHolder*>* algo) : algo_(algo) {

}
OptimAlgoI<DataHolder*>* QAlgoWrapper::getAlgo()
{
	return algo_;
}

void QAlgoWrapper::setAlgorithm(OptimAlgoI<DataHolder*>* alg)
{
	algo_ = alg;
}

void QAlgoWrapper::RunOneStep()
{
	if (algo_)algo_->execute(1);
}

void QAlgoWrapper::Reset()
{
	algo_->reset();
}

void QAlgoWrapper::RunUntilTermination() {
	if (algo_)algo_->execute(AlgorithmConstants::maxIterations);
}