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

	QPushButton* getStartButton() const;
	QComboBox* getAlgoSelectionBox() const;

	QLabel* getOptimNumberLabel() const;
	QLabel* getCurrentNumberLabel() const;
	QLabel* getCurrentOptimPctLabel() const;

	QLabel* getCurrentStepLabelNumber() const;
	QPushButton* getNextStepButton() const;

	QPushButton* getResetButton() const;
public slots:
	void setCurrentNumberLabel(int number);
	void setOptimNumberLabel(float number);
	void setCurrentOptimPctLabel(float number);
	void setCurrentStepNumberLabel(int number);
	void setTimeDisplayLabel(double number);
	void setAvgTimeDisplayLabel(double number);
private:
	QBoxLayout* layout_;
	QGridLayout* topLayout_;
	QGridLayout* bottomLayout_;
	QLabel* recAmountText_;
	QLabel* recMaxSizeText_;
	QSlider* recAmountInput_;
	QSlider* recMaxSizeInput_;
	QLineEdit* recAmountAltInput_;
	QLineEdit* recMaxSizeAltInput_;
	QLabel* boxSizeText_;
	QSlider* boxSizeSlider_;
	QLineEdit* boxSizeLineEdit_;

	QPushButton* startButton_;
	QComboBox* algoSelectionBox_;

	QLabel* optimLabel_;
	QLabel* currentLabel_;

	QLabel* optimNumberLabel_;
	QLabel* currentNumberLabel_;
	QLabel* currentOptimPctLabel_;

	QPushButton* nextStepButton_;
	QLabel* currentStepLabel_;
	QLabel* currentStepLabelNumber_;

	QPushButton* resetButton_;

	QLabel* timeTextLabel_;
	QLabel* timeDisplayLabel_;
	QLabel* avgTimeDisplayLabel_;

	
};