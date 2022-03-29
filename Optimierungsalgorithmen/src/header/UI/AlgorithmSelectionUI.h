#pragma once
#include "stdafx.h"

class AlgorithmSelectionUI : public QWidget {
	Q_OBJECT
public:
	AlgorithmSelectionUI();
	~AlgorithmSelectionUI();
	QSlider* getRecAmountSlider() const;
	QLineEdit* getRecAmountLineEdit() const;

	QSlider* getRecMinWidthSlider() const;
	QLineEdit* getRecMinWidthLineEdit() const;

	QSlider* getRecMaxWidthSlider() const;
	QLineEdit* getRecMaxWidthLineEdit() const;

	QSlider* getRecMinHeightSlider() const;
	QLineEdit* getRecMinHeightLineEdit() const;

	QSlider* getRecMaxHeightSlider() const;
	QLineEdit* getRecMaxHeightLineEdit() const;

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
signals:
	void EmitMaxWidthChangedI(const int w);
	void EmitMaxWidthChangedS(const QString& w);

	void EmitMaxHeightChangedI(const int w);
	void EmitMaxHeightChangedS(const QString& w);

	/*
	void EmitMinWidthChangedI(const int w);
	void EmitMinWidthChangedS(const QString& w);

	void EmitMinWidthChangedI(const int w);
	void EmitMinWidthChangedS(const QString& w);
	*/

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
	QGridLayout* middleLayout_;
	QGridLayout* bottomLayout_;
	QLabel* recAmountText_;

	QLabel* recMinWidthText_;
	QSlider* recMinWidthInput_;
	QLineEdit* recMinWidthAltInput_;

	QLabel* recMaxWidthText_;
	QSlider* recMaxWidthInput_;
	QLineEdit* recMaxWidthAltInput_;

	QLabel* recMinHeightText_;
	QSlider* recMinHeightInput_;
	QLineEdit* recMinHeightAltInput_;

	QLabel* recMaxHeightText_;
	QSlider* recMaxHeightInput_;
	QLineEdit* recMaxHeightAltInput_;

	QSlider* recAmountInput_;
	QLineEdit* recAmountAltInput_;
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