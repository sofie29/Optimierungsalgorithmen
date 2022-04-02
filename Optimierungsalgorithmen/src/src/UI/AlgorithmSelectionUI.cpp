#include "AlgorithmSelectionUI.h"

AlgorithmSelectionUI::AlgorithmSelectionUI()
{
	layout_ = new QBoxLayout(QBoxLayout::TopToBottom, this);
	layout_->setAlignment(Qt::AlignCenter|Qt::AlignTop);

	
	topLayout_ = new QGridLayout();
	layout_->addLayout(topLayout_);
	topLayout_->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	middleLayout_ = new QGridLayout();
	layout_->addLayout(middleLayout_);
	middleLayout_->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	bottomLayout_ = new QGridLayout();
	layout_->addLayout(bottomLayout_);
	bottomLayout_->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	QFont f("Arial", 10, QFont::Bold);
	recAmountText_ = new QLabel();
	recAmountText_->setFont(f);
	recAmountText_->setText("Amount of Rectangles: ");
	topLayout_->addWidget(recAmountText_, 0, 0, Qt::AlignLeft);

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


	recMinWidthText_ = new QLabel();
	recMinWidthText_->setFont(f);
	recMinWidthText_->setText("Min Rectangle Edge Width: ");
	topLayout_->addWidget(recMinWidthText_, 1, 0, Qt::AlignLeft);

	recMaxWidthText_ = new QLabel();
	recMaxWidthText_->setFont(f);
	recMaxWidthText_->setText("Max Rectangle Edge Width: ");
	topLayout_->addWidget(recMaxWidthText_, 2, 0, Qt::AlignLeft);

	recMinHeightText_ = new QLabel();
	recMinHeightText_->setFont(f);
	recMinHeightText_->setText("Min Rectangle Edge Height: ");
	topLayout_->addWidget(recMinHeightText_, 3, 0, Qt::AlignLeft);

	recMaxHeightText_ = new QLabel();
	recMaxHeightText_->setFont(f);
	recMaxHeightText_->setText("Max Rectangle Edge Height: ");
	topLayout_->addWidget(recMaxHeightText_, 4, 0, Qt::AlignLeft);

	recMinWidthInput_ = new QSlider(Qt::Horizontal);
	recMinWidthInput_->setTickPosition(QSlider::TicksBelow);
	recMinWidthInput_->setTickInterval(1);
	recMinWidthInput_->setMaximum(AlgorithmConstants::maxRectangleEdgeSize_);
	recMinWidthInput_->setMinimum(AlgorithmConstants::minRectangleEdgeSize_);
	recMinWidthInput_->setValue(AlgorithmConstants::initialEdgeSize_);
	topLayout_->addWidget(recMinWidthInput_, 1, 1, Qt::AlignLeft);

	recMinWidthAltInput_ = new QLineEdit();
	recMinWidthAltInput_->setFont(f);
	recMinWidthAltInput_->setText(QString::number(recMinWidthInput_->value()));
	recMinWidthAltInput_->setValidator(new QIntValidator(AlgorithmConstants::minRectangleEdgeSize_, AlgorithmConstants::maxRectangleEdgeSize_, recMinWidthAltInput_));
	topLayout_->addWidget(recMinWidthAltInput_, 1, 2, Qt::AlignLeft);
	connect(recMinWidthInput_, &QSlider::valueChanged, recMinWidthAltInput_, [=]() {recMinWidthAltInput_->setText(QString::number(recMinWidthInput_->value())); });
	connect(recMinWidthAltInput_, &QLineEdit::textChanged, recMinWidthInput_, [=]() {recMinWidthInput_->setValue(recMinWidthAltInput_->text().toInt()); });

	recMaxWidthInput_ = new QSlider(Qt::Horizontal);
	recMaxWidthInput_->setTickPosition(QSlider::TicksBelow);
	recMaxWidthInput_->setTickInterval(1);
	recMaxWidthInput_->setMaximum(AlgorithmConstants::maxRectangleEdgeSize_);
	recMaxWidthInput_->setMinimum(AlgorithmConstants::minRectangleEdgeSize_);
	recMaxWidthInput_->setValue(AlgorithmConstants::initialEdgeSize_);
	topLayout_->addWidget(recMaxWidthInput_, 2, 1, Qt::AlignLeft);

	recMaxWidthAltInput_ = new QLineEdit();
	recMaxWidthAltInput_->setFont(f);
	recMaxWidthAltInput_->setText(QString::number(recMaxWidthInput_->value()));
	recMaxWidthAltInput_->setValidator(new QIntValidator(AlgorithmConstants::minRectangleEdgeSize_, AlgorithmConstants::maxRectangleEdgeSize_, recMaxWidthAltInput_));
	topLayout_->addWidget(recMaxWidthAltInput_, 2, 2, Qt::AlignLeft);
	connect(recMaxWidthInput_, &QSlider::valueChanged, recMaxWidthAltInput_, [=]() {recMaxWidthAltInput_->setText(QString::number(recMaxWidthInput_->value())); });
	connect(recMaxWidthAltInput_, &QLineEdit::textChanged, recMaxWidthInput_, [=]() {recMaxWidthInput_->setValue(recMaxWidthAltInput_->text().toInt()); });

	recMinHeightInput_ = new QSlider(Qt::Horizontal);
	recMinHeightInput_->setTickPosition(QSlider::TicksBelow);
	recMinHeightInput_->setTickInterval(1);
	recMinHeightInput_->setMaximum(AlgorithmConstants::maxRectangleEdgeSize_);
	recMinHeightInput_->setMinimum(AlgorithmConstants::minRectangleEdgeSize_);
	recMinHeightInput_->setValue(AlgorithmConstants::initialEdgeSize_);
	topLayout_->addWidget(recMinHeightInput_, 3, 1, Qt::AlignLeft);

	recMinHeightAltInput_ = new QLineEdit();
	recMinHeightAltInput_->setFont(f);
	recMinHeightAltInput_->setText(QString::number(recMinHeightInput_->value()));
	recMinHeightAltInput_->setValidator(new QIntValidator(AlgorithmConstants::minRectangleEdgeSize_, AlgorithmConstants::maxRectangleEdgeSize_, recMinHeightAltInput_));
	topLayout_->addWidget(recMinHeightAltInput_, 3, 2, Qt::AlignLeft);
	connect(recMinHeightInput_, &QSlider::valueChanged, recMinHeightAltInput_, [=]() {recMinHeightAltInput_->setText(QString::number(recMinHeightInput_->value())); });
	connect(recMinHeightAltInput_, &QLineEdit::textChanged, recMinHeightInput_, [=]() {recMinHeightInput_->setValue(recMinHeightAltInput_->text().toInt()); });

	recMaxHeightInput_ = new QSlider(Qt::Horizontal);
	recMaxHeightInput_->setTickPosition(QSlider::TicksBelow);
	recMaxHeightInput_->setTickInterval(1);
	recMaxHeightInput_->setMaximum(AlgorithmConstants::maxRectangleEdgeSize_);
	recMaxHeightInput_->setMinimum(AlgorithmConstants::minRectangleEdgeSize_);
	recMaxHeightInput_->setValue(AlgorithmConstants::initialEdgeSize_);
	topLayout_->addWidget(recMaxHeightInput_, 4, 1, Qt::AlignLeft);

	recMaxHeightAltInput_ = new QLineEdit();
	recMaxHeightAltInput_->setFont(f);
	recMaxHeightAltInput_->setText(QString::number(recMaxHeightInput_->value()));
	recMaxHeightAltInput_->setValidator(new QIntValidator(AlgorithmConstants::minRectangleEdgeSize_, AlgorithmConstants::maxRectangleEdgeSize_, recMaxHeightAltInput_));
	topLayout_->addWidget(recMaxHeightAltInput_, 4, 2, Qt::AlignLeft);
	connect(recMaxHeightInput_, &QSlider::valueChanged, recMaxHeightAltInput_, [=]() {recMaxHeightAltInput_->setText(QString::number(recMaxHeightInput_->value())); });
	connect(recMaxHeightAltInput_, &QLineEdit::textChanged, recMaxHeightInput_, [=]() {recMaxHeightInput_->setValue(recMaxHeightAltInput_->text().toInt()); });

	connect(recMaxHeightInput_, &QSlider::valueChanged, this,
		[=](int val) {recMinHeightInput_->setMaximum(val); recMinHeightInput_->setValue(std::min(recMinHeightInput_->value(), val));
	recMinHeightAltInput_->setValidator(new QIntValidator(AlgorithmConstants::minRectangleEdgeSize_, val, recMinHeightAltInput_));
	recMinHeightAltInput_->setText(QString::number(std::min(recMinHeightAltInput_->text().toInt(), val))); emit EmitMaxHeightChangedI(val); });

	connect(recMaxHeightAltInput_, &QLineEdit::textChanged, this,
		[=](const QString& val) {recMinHeightInput_->setMaximum(val.toInt()); recMinHeightInput_->setValue(std::min(recMinHeightInput_->value(), val.toInt()));
	recMinHeightAltInput_->setValidator(new QIntValidator(AlgorithmConstants::minRectangleEdgeSize_, val.toInt(), recMinHeightAltInput_));
	recMinHeightAltInput_->setText(QString::number(std::min(recMinHeightAltInput_->text().toInt(), val.toInt()))); emit EmitMaxHeightChangedS(val); });
	
	connect(recMaxWidthInput_, &QSlider::valueChanged, this,
		[=](int val) {recMinWidthInput_->setMaximum(val); recMinWidthInput_->setValue(std::min(recMinWidthInput_->value(), val));
	recMinWidthAltInput_->setValidator(new QIntValidator(AlgorithmConstants::minRectangleEdgeSize_, val, recMinWidthAltInput_));
	recMinWidthAltInput_->setText(QString::number(std::min(recMinWidthAltInput_->text().toInt(), val))); emit EmitMaxWidthChangedI(val); });

	connect(recMaxWidthAltInput_, &QLineEdit::textChanged, this,
		[=](const QString& val) {recMinWidthInput_->setMaximum(val.toInt()); recMinWidthInput_->setValue(std::min(recMinWidthInput_->value(), val.toInt()));
	recMinWidthAltInput_->setValidator(new QIntValidator(AlgorithmConstants::minRectangleEdgeSize_, val.toInt(), recMinWidthAltInput_));
	recMinWidthAltInput_->setText(QString::number(std::min(recMinWidthAltInput_->text().toInt(), val.toInt()))); emit EmitMaxWidthChangedS(val); });


	boxSizeText_ = new QLabel();
	boxSizeText_->setFont(f);
	boxSizeText_->setText("Box Edge Length: ");
	topLayout_->addWidget(boxSizeText_, 5, 0, Qt::AlignLeft);

	boxSizeSlider_ = new QSlider(Qt::Horizontal);
	boxSizeSlider_->setTickPosition(QSlider::TicksBelow);
	boxSizeSlider_->setTickInterval(1);
	boxSizeSlider_->setMaximum(AlgorithmConstants::maxBoxEdgeSize_);
	boxSizeSlider_->setMinimum(AlgorithmConstants::maxRectangleEdgeSize_);
	boxSizeSlider_->setValue(AlgorithmConstants::initialEdgeSize_);
	topLayout_->addWidget(boxSizeSlider_, 5, 1, Qt::AlignLeft);

	boxSizeLineEdit_ = new QLineEdit();
	boxSizeLineEdit_->setFont(f);
	boxSizeLineEdit_->setText(QString::number(boxSizeSlider_->value()));
	boxSizeLineEdit_->setValidator(new QIntValidator(AlgorithmConstants::maxRectangleEdgeSize_, AlgorithmConstants::maxBoxEdgeSize_, boxSizeLineEdit_));
	topLayout_->addWidget(boxSizeLineEdit_, 5, 2, Qt::AlignLeft);
	connect(boxSizeSlider_, &QSlider::valueChanged, boxSizeLineEdit_, [=]() {boxSizeLineEdit_->setText(QString::number(boxSizeSlider_->value())); });
	connect(boxSizeLineEdit_, &QLineEdit::textChanged, boxSizeSlider_, [=]() {boxSizeSlider_->setValue(boxSizeLineEdit_->text().toInt()); });

	startButton_ = new QPushButton("All Iterations");
	middleLayout_->addWidget(startButton_, 0, 1, Qt::AlignLeft);

	algoSelectionBox_ = new QComboBox();
	algoSelectionBox_->addItem("RuleBasedNeighbour");
	algoSelectionBox_->addItem("Geometry");
	algoSelectionBox_->addItem("GeometryOverlapping");
	algoSelectionBox_->addItem("Greedy Area Sort");
	algoSelectionBox_->addItem("Greedy Diagonal Sort");
	middleLayout_->addWidget(algoSelectionBox_,0, 0, Qt::AlignLeft);

	nextStepButton_ = new QPushButton("Next Step");
	middleLayout_->addWidget(nextStepButton_, 0, 2, Qt::AlignLeft);

	currentStepLabel_ = new QLabel("Current Step: ");
	middleLayout_->addWidget(currentStepLabel_, 0, 3, Qt::AlignLeft);

	currentStepLabelNumber_ = new QLabel("0");
	middleLayout_->addWidget(currentStepLabelNumber_, 0, 4, Qt::AlignLeft);

	resetButton_ = new QPushButton("Reset");
	middleLayout_->addWidget(resetButton_, 0, 6, Qt::AlignLeft);


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


	delete recMinHeightInput_;
	recMinHeightInput_ = nullptr;

	delete recMinHeightAltInput_;
	recMinHeightAltInput_ = nullptr;

	delete recMinHeightText_;
	recMinHeightText_ = nullptr;

	delete recMaxHeightInput_;
	recMaxHeightInput_ = nullptr;

	delete recMaxHeightAltInput_;
	recMaxHeightAltInput_ = nullptr;

	delete recMaxHeightText_;
	recMaxHeightText_ = nullptr;

	delete recMinWidthInput_;
	recMinWidthInput_ = nullptr;

	delete recMaxWidthAltInput_;
	recMaxWidthAltInput_ = nullptr;

	delete recMaxWidthInput_;
	recMaxWidthInput_ = nullptr;

	delete recMaxWidthText_;
	recMaxWidthText_ = nullptr;

	delete recMinWidthAltInput_;
	recMinWidthAltInput_ = nullptr;

	delete recMinWidthText_;
	recMinWidthText_ = nullptr;

	delete recAmountInput_;
	recAmountInput_ = nullptr;

	delete recAmountAltInput_;
	recAmountAltInput_ = nullptr;

	delete recAmountText_;
	recAmountText_ = nullptr;

	delete topLayout_;
	topLayout_ = nullptr;

	delete middleLayout_;
	middleLayout_ = nullptr;

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


QLineEdit* AlgorithmSelectionUI::getRecAmountLineEdit() const
{
	return recAmountAltInput_;
}

QSlider* AlgorithmSelectionUI::getRecMinWidthSlider() const
{
	return recMinWidthInput_;
}

QLineEdit* AlgorithmSelectionUI::getRecMinWidthLineEdit() const
{
	return recMinWidthAltInput_;
}

QSlider* AlgorithmSelectionUI::getRecMaxWidthSlider() const
{
	return recMaxWidthInput_;
}

QLineEdit* AlgorithmSelectionUI::getRecMaxWidthLineEdit() const
{
	return recMaxWidthAltInput_;
}

QSlider* AlgorithmSelectionUI::getRecMinHeightSlider() const
{
	return recMinHeightInput_;
}

QLineEdit* AlgorithmSelectionUI::getRecMinHeightLineEdit() const
{
	return recMinHeightAltInput_;
}

QSlider* AlgorithmSelectionUI::getRecMaxHeightSlider() const
{
	return recMaxHeightInput_;
}

QLineEdit* AlgorithmSelectionUI::getRecMaxHeightLineEdit() const
{
	return recMaxHeightAltInput_;
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

