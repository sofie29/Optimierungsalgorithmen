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

	QSlider* getBoxEdgeSlider() const;
	QLineEdit* getBoxEdgeLineEdit() const;
signals:
	void OnValueChanged();
public slots:
	void ValueChangedI(int dummy);
	void ValueChangedS(QString dummy);
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