#pragma once

namespace AlgorithmConstants {
	static const int maxRectangleAmount_ = 1000;
	static const int maxRectangleEdgeSize_ = 50;
	static const int minRectangleEdgeSize_ = 10;

	static const int initialAmount_ = 10;
	static const int initialEdgeSize_ = 50;

	static const int maxBoxEdgeSize_ = 2 * maxRectangleEdgeSize_;
	static const int initialBoxEdgeSize_ = initialEdgeSize_;

	static const int  maxIterations = 10000;
	static const int  maxIterationsUI_ = 10000000;
	static const int  maxScore = 999000; // upper bound for score in geometry based neighbourhoods
	

	static constexpr double maxTime_ = 10000.0;
	static constexpr double timeOverhead_ = 100.0;

	static const int maxInitialRectsPerBox_ = 1;

	static const int amountAlgorithms_ = 5;
}