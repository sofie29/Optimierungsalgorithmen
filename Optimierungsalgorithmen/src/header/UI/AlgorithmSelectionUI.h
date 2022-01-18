#pragma once
#include "stdafx.h"

class AlgorithmSelectionUI : public QWidget {
	Q_OBJECT
public:
	AlgorithmSelectionUI();
	~AlgorithmSelectionUI();
	QSlider* getRecAmountSlider() const;
	QSlider* getRecMaxSizeSlider() const;
	QLineEdit* getRecAmountLineEdit() const;
	QLineEdit* getRecMaxSizeLineEdit() const;
private:
	QBoxLayout* layout_;
	QGridLayout* gridLayout_;
	QLabel* recAmountText_;
	QLabel* recMaxSizeText_;
	QSlider* recAmountInput_;
	QSlider* recMaxSizeInput_;
	QLineEdit* recAmountAltInput_;
	QLineEdit* recMaxSizeAltInput_;

	QLabel* boxSizeText_;
	QSlider* boxSizeSlider_;
	QLineEdit* boxSizeLineEdit_;

	
};