#include "AlgorithmSelectionUI.h"

AlgorithmSelectionUI::AlgorithmSelectionUI()
{
	layout_ = new QBoxLayout(QBoxLayout::TopToBottom, this);
	layout_->setAlignment(Qt::AlignCenter|Qt::AlignTop);

	
	topLayout_ = new QGridLayout();
	layout_->addLayout(topLayout_);
	topLayout_->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	bottomLayout_ = new QGridLayout();
	layout_->addLayout(bottomLayout_);
	bottomLayout_->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	QFont f("Arial", 10, QFont::Bold);
	recAmountText_ = new QLabel();
	recAmountText_->setFont(f);
	recAmountText_->setText("Amount of Rectangles: ");
	topLayout_->addWidget(recAmountText_, 0, 0, Qt::AlignLeft);

	recMaxSizeText_ = new QLabel();
	recMaxSizeText_->setFont(f);
	recMaxSizeText_->setText("Max Rectangle Edge Length: ");
	topLayout_->addWidget(recMaxSizeText_, 1, 0, Qt::AlignLeft);

	recAmountInput_ = new QSlider(Qt::Horizontal);
	recAmountInput_->setTickPosition(QSlider::TicksBelow);
	recAmountInput_->setTickInterval(1);
	recAmountInput_->setMaximum(AlgorithmConstants::maxRectangleAmount_);
	recAmountInput_->setMinimum(1);
	recAmountInput_->setValue(AlgorithmConstants::initialAmount_);
	topLayout_->addWidget(recAmountInput_, 0, 1, Qt::AlignLeft);

	recAmountAltInput_ = new QLineEdit();
	recAmountAltInput_->setFont(f);
	recAmountAltInput_->setText(QString::number(recAmountInput_->value()));
	recAmountAltInput_->setValidator(new QIntValidator(1, AlgorithmConstants::maxRectangleAmount_, recAmountAltInput_));
	topLayout_->addWidget(recAmountAltInput_, 0, 2, Qt::AlignLeft);
	connect(recAmountInput_, &QSlider::valueChanged, recAmountAltInput_, [=]() {recAmountAltInput_->setText(QString::number(recAmountInput_->value())); });
	connect(recAmountAltInput_, &QLineEdit::textChanged, recAmountInput_, [=]() {recAmountInput_->setValue(recAmountAltInput_->text().toInt()); });


	recMaxSizeInput_ = new QSlider(Qt::Horizontal);
	recMaxSizeInput_->setTickPosition(QSlider::TicksBelow);
	recMaxSizeInput_->setTickInterval(1);
	recMaxSizeInput_->setMaximum(AlgorithmConstants::maxRectangleEdgeSize_);
	recMaxSizeInput_->setMinimum(AlgorithmConstants::minRectangleEdgeSize_);
	recMaxSizeInput_->setValue(AlgorithmConstants::initialEdgeSize_);
	topLayout_->addWidget(recMaxSizeInput_, 1, 1, Qt::AlignLeft);

	recMaxSizeAltInput_ = new QLineEdit();
	recMaxSizeAltInput_->setFont(f);
	recMaxSizeAltInput_->setText(QString::number(recMaxSizeInput_->value()));
	recMaxSizeAltInput_->setValidator(new QIntValidator(AlgorithmConstants::minRectangleEdgeSize_, AlgorithmConstants::maxRectangleEdgeSize_, recMaxSizeAltInput_));
	topLayout_->addWidget(recMaxSizeAltInput_, 1, 2, Qt::AlignLeft);
	connect(recMaxSizeInput_, &QSlider::valueChanged, recMaxSizeAltInput_, [=]() {recMaxSizeAltInput_->setText(QString::number(recMaxSizeInput_->value())); });
	connect(recMaxSizeAltInput_, &QLineEdit::textChanged, recMaxSizeInput_, [=]() {recMaxSizeInput_->setValue(recMaxSizeAltInput_->text().toInt()); });

	boxSizeText_ = new QLabel();
	boxSizeText_->setFont(f);
	boxSizeText_->setText("Box Edge Length: ");
	topLayout_->addWidget(boxSizeText_, 2, 0, Qt::AlignLeft);

	boxSizeSlider_ = new QSlider(Qt::Horizontal);
	boxSizeSlider_->setTickPosition(QSlider::TicksBelow);
	boxSizeSlider_->setTickInterval(1);
	boxSizeSlider_->setMaximum(AlgorithmConstants::maxBoxEdgeSize_);
	boxSizeSlider_->setMinimum(AlgorithmConstants::maxRectangleEdgeSize_);
	boxSizeSlider_->setValue(AlgorithmConstants::initialEdgeSize_);
	topLayout_->addWidget(boxSizeSlider_, 2, 1, Qt::AlignLeft);

	boxSizeLineEdit_ = new QLineEdit();
	boxSizeLineEdit_->setFont(f);
	boxSizeLineEdit_->setText(QString::number(boxSizeSlider_->value()));
	boxSizeLineEdit_->setValidator(new QIntValidator(AlgorithmConstants::maxRectangleEdgeSize_, AlgorithmConstants::maxBoxEdgeSize_, boxSizeLineEdit_));
	topLayout_->addWidget(boxSizeLineEdit_, 2, 2, Qt::AlignLeft);
	connect(boxSizeSlider_, &QSlider::valueChanged, boxSizeLineEdit_, [=]() {boxSizeLineEdit_->setText(QString::number(boxSizeSlider_->value())); });
	connect(boxSizeLineEdit_, &QLineEdit::textChanged, boxSizeSlider_, [=]() {boxSizeSlider_->setValue(boxSizeLineEdit_->text().toInt()); });

	startButton_ = new QPushButton("All Iterations");
	topLayout_->addWidget(startButton_, 3, 1, Qt::AlignLeft);

	algoSelectionBox_ = new QComboBox();
	algoSelectionBox_->addItem("RuleBasedNeighbour");
	algoSelectionBox_->addItem("Geometry");
	algoSelectionBox_->addItem("GeometryOverlapping");
	algoSelectionBox_->addItem("Greedy");
	topLayout_->addWidget(algoSelectionBox_, 3, 0, Qt::AlignLeft);

	nextStepButton_ = new QPushButton("Next Step");
	topLayout_->addWidget(nextStepButton_, 3, 2, Qt::AlignLeft);

	currentStepLabel_ = new QLabel("Current Step: ");
	topLayout_->addWidget(currentStepLabel_, 3, 3, Qt::AlignLeft);

	currentStepLabelNumber_ = new QLabel("0");
	topLayout_->addWidget(currentStepLabelNumber_, 3, 4, Qt::AlignLeft);

	resetButton_ = new QPushButton("Reset");
	topLayout_->addWidget(resetButton_, 3, 6, Qt::AlignLeft);


	optimLabel_ = new QLabel("Minimum number of bounding boxes required: ");
	bottomLayout_->addWidget(optimLabel_, 0, 0, Qt::AlignLeft);

	currentLabel_ = new QLabel("Current number of bounding boxes: ");
	bottomLayout_->addWidget(currentLabel_, 1, 0, Qt::AlignLeft);

	optimNumberLabel_ = new QLabel("0.00");
	bottomLayout_->addWidget(optimNumberLabel_, 0, 1, Qt::AlignLeft);

	currentNumberLabel_ = new QLabel("0");
	bottomLayout_->addWidget(currentNumberLabel_, 1, 1, Qt::AlignLeft);

	currentOptimPctLabel_ = new QLabel("0.00");
	bottomLayout_->addWidget(currentOptimPctLabel_, 1, 2, Qt::AlignLeft);

	timeTextLabel_ = new QLabel("Computation time until now: ");
	bottomLayout_->addWidget(timeTextLabel_, 3, 0, Qt::AlignLeft);

	timeDisplayLabel_ = new QLabel("0.00ms");
	bottomLayout_->addWidget(timeDisplayLabel_, 3, 1, Qt::AlignLeft);

	avgTimeDisplayLabel_ = new QLabel("(0.00ms per step)");
	bottomLayout_->addWidget(avgTimeDisplayLabel_, 3, 2, Qt::AlignLeft);
}

AlgorithmSelectionUI::~AlgorithmSelectionUI()
{
	delete avgTimeDisplayLabel_;
	avgTimeDisplayLabel_ = nullptr;

	delete timeTextLabel_;
	timeTextLabel_ = nullptr;

	delete timeDisplayLabel_;
	timeDisplayLabel_ = nullptr;

	delete resetButton_;
	resetButton_ = nullptr;

	delete currentStepLabel_;
	currentStepLabel_ = nullptr;

	delete currentStepLabelNumber_;
	currentStepLabelNumber_ = nullptr;

	delete nextStepButton_;
	nextStepButton_ = nullptr;

	delete currentOptimPctLabel_;
	currentOptimPctLabel_ = nullptr;

	delete currentNumberLabel_;
	currentNumberLabel_ = nullptr;

	delete optimNumberLabel_;
	optimNumberLabel_ = nullptr;

	delete optimLabel_;
	optimLabel_ = nullptr;

	delete currentLabel_;
	currentLabel_ = nullptr;

	delete algoSelectionBox_;
	algoSelectionBox_ = nullptr;

	delete startButton_;
	startButton_ = nullptr;

	delete boxSizeText_;
	boxSizeText_ = nullptr;

	delete boxSizeSlider_;
	boxSizeSlider_ = nullptr;

	delete boxSizeLineEdit_;
	boxSizeLineEdit_ = nullptr;

	delete recMaxSizeAltInput_;
	recMaxSizeAltInput_ = nullptr;

	delete recAmountAltInput_;
	recAmountAltInput_ = nullptr;

	delete recMaxSizeInput_;
	recMaxSizeInput_ = nullptr;

	delete recAmountInput_;
	recAmountInput_ = nullptr;

	delete recMaxSizeText_;
	recMaxSizeText_ = nullptr;

	delete recAmountText_;
	recAmountText_ = nullptr;

	delete topLayout_;
	topLayout_ = nullptr;

	delete bottomLayout_;
	bottomLayout_ = nullptr;

	//delete gridLayout_;
	//gridLayout_ = nullptr;

	delete layout_;
	layout_ = nullptr;
}

QSlider* AlgorithmSelectionUI::getRecAmountSlider() const
{
	return recAmountInput_;
}

QSlider* AlgorithmSelectionUI::getRecMaxSizeSlider() const
{
	return recMaxSizeInput_;
}

QLineEdit* AlgorithmSelectionUI::getRecAmountLineEdit() const
{
	return recAmountAltInput_;
}

QLineEdit* AlgorithmSelectionUI::getRecMaxSizeLineEdit() const
{
	return recMaxSizeAltInput_;
}

QSlider* AlgorithmSelectionUI::getBoxEdgeSlider() const
{
	return boxSizeSlider_;
}

QLineEdit* AlgorithmSelectionUI::getBoxEdgeLineEdit() const
{
	return boxSizeLineEdit_;
}

QPushButton* AlgorithmSelectionUI::getStartButton() const
{
	return startButton_;
}

QComboBox* AlgorithmSelectionUI::getAlgoSelectionBox() const
{
	return algoSelectionBox_;
}

QLabel* AlgorithmSelectionUI::getOptimNumberLabel() const
{
	return optimNumberLabel_;
}

QLabel* AlgorithmSelectionUI::getCurrentNumberLabel() const
{
	return currentNumberLabel_;
}
QLabel* AlgorithmSelectionUI::getCurrentOptimPctLabel() const
{
	return currentOptimPctLabel_;
}

QLabel* AlgorithmSelectionUI::getCurrentStepLabelNumber() const
{
	return currentStepLabelNumber_;
}

QPushButton* AlgorithmSelectionUI::getNextStepButton() const
{
	return nextStepButton_;
}

QPushButton* AlgorithmSelectionUI::getResetButton() const
{
	return resetButton_;
}

void AlgorithmSelectionUI::setCurrentNumberLabel(int number)
{
	QString s = QString::number(number);
	currentNumberLabel_->setText(s);
}

void AlgorithmSelectionUI::setOptimNumberLabel(float number)
{
	QString s = QString::number(number, 'f', 2);
	optimNumberLabel_->setText(s);
}

void AlgorithmSelectionUI::setCurrentOptimPctLabel(float number)
{
	float num = number * 100;
	QString s = QString::number(num, 'f', 2) + QString("% optimal");
	currentOptimPctLabel_->setText(s);
}

void AlgorithmSelectionUI::setCurrentStepNumberLabel(int number)
{
	QString s = QString::number(number);
	currentStepLabelNumber_->setText(s);
	
}

void AlgorithmSelectionUI::setTimeDisplayLabel(double number)
{
	QString s = QString::number(number, 'f', 2) + QString("ms");
	timeDisplayLabel_->setText(s);
}

void AlgorithmSelectionUI::setAvgTimeDisplayLabel(double number)
{
	QString s = QString("(") + QString::number(number, 'f', 2) + QString("ms per step)");
	avgTimeDisplayLabel_->setText(s);
}

