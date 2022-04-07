#include "QAlgoWrapper.h"
#include "DataHolderT.h"
#include "OptimAlgoI.h"
QAlgoWrapper::QAlgoWrapper(class OptimAlgoI<class DataHolder*>* algo) : algo_(algo) {
	stepIdx_ = 0;
	mod_ = 2;
	isDone_ = false;
}
OptimAlgoI<DataHolder*>* QAlgoWrapper::getAlgo()
{
	return algo_;
}

void QAlgoWrapper::setAlgorithm(OptimAlgoI<DataHolder*>* alg)
{
	algo_ = alg;
}

void QAlgoWrapper::setMod(int mod)
{
	mod_ = mod;
}

void QAlgoWrapper::RunOneStep()
{
	int rmd = stepIdx_ % mod_;
	
	if (algo_) {
		switch (rmd) {
		case 0:
		{
			Metric m = algo_->execute(1);
			isDone_ = m.isDone_;
			if (mod_ > 1) {
				//draw flipped rects
				emit algo_->DrawSwappedRects();
			}
			else {
				//draw all
				emit algo_->DrawSolution(algo_->getBestSol()->getData()->getBoxCreator().get(), isDone_);
			}
		}
			break;
		
		case 1:
			emit algo_->DrawSolution(algo_->getBestSol()->getData()->getBoxCreator().get(), isDone_);
			break;
		}
		
	}
	stepIdx_++;
}

void QAlgoWrapper::Reset()
{
	algo_->reset();
	stepIdx_ = 0;
	emit algo_->DrawSolution(algo_->getBestSol()->getData()->getBoxCreator().get(), false);
}

void QAlgoWrapper::RunUntilTermination() {
	if (algo_) {
		algo_->execute(AlgorithmConstants::maxIterationsUI_);
		emit algo_->DrawSolution(algo_->getBestSol()->getData()->getBoxCreator().get(), true);
	}
}