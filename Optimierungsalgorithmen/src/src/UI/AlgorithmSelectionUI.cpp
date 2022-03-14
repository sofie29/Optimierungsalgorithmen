#include "AlgorithmSelectionUI.h"

AlgorithmSelectionUI::AlgorithmSelectionUI()
{
	layout_ = new QBoxLayout(QBoxLayout::TopToBottom, this);
	layout_->setAlignment(Qt::AlignCenter|Qt::AlignTop);

	gridLayout_ = new QGridLayout();
	layout_->addLayout(gridLayout_);
	gridLayout_->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	QFont f("Arial", 10, QFont::Bold);
	recAmountText_ = new QLabel();
	recAmountText_->setFont(f);
	recAmountText_->setText("Amount of Rectangles: ");
	gridLayout_->addWidget(recAmountText_, 0, 0, Qt::AlignLeft);

	recMaxSizeText_ = new QLabel();
	recMaxSizeText_->setFont(f);
	recMaxSizeText_->setText("Max Rectangle Edge Length: ");
	gridLayout_->addWidget(recMaxSizeText_, 1, 0, Qt::AlignLeft);

	recAmountInput_ = new QSlider(Qt::Horizontal);
	recAmountInput_->setTickPosition(QSlider::TicksBelow);
	recAmountInput_->setTickInterval(1);
	recAmountInput_->setMaximum(AlgorithmConstants::maxRectangleAmount_);
	recAmountInput_->setMinimum(1);
	recAmountInput_->setValue(AlgorithmConstants::initialAmount_);
	gridLayout_->addWidget(recAmountInput_, 0, 1, Qt::AlignLeft);

	recAmountAltInput_ = new QLineEdit();
	recAmountAltInput_->setFont(f);
	recAmountAltInput_->setText(QString::number(recAmountInput_->value()));
	recAmountAltInput_->setValidator(new QIntValidator(1, AlgorithmConstants::maxRectangleAmount_, recAmountAltInput_));
	gridLayout_->addWidget(recAmountAltInput_, 0, 2, Qt::AlignLeft);
	connect(recAmountInput_, &QSlider::valueChanged, recAmountAltInput_, [=]() {recAmountAltInput_->setText(QString::number(recAmountInput_->value())); });
	connect(recAmountAltInput_, &QLineEdit::textChanged, recAmountInput_, [=]() {recAmountInput_->setValue(recAmountAltInput_->text().toInt()); });


	recMaxSizeInput_ = new QSlider(Qt::Horizontal);
	recMaxSizeInput_->setTickPosition(QSlider::TicksBelow);
	recMaxSizeInput_->setTickInterval(1);
	recMaxSizeInput_->setMaximum(AlgorithmConstants::maxRectangleEdgeSize_);
	recMaxSizeInput_->setMinimum(AlgorithmConstants::minRectangleEdgeSize_);
	recMaxSizeInput_->setValue(AlgorithmConstants::initialEdgeSize_);
	gridLayout_->addWidget(recMaxSizeInput_, 1, 1, Qt::AlignLeft);

	recMaxSizeAltInput_ = new QLineEdit();
	recMaxSizeAltInput_->setFont(f);
	recMaxSizeAltInput_->setText(QString::number(recMaxSizeInput_->value()));
	recMaxSizeAltInput_->setValidator(new QIntValidator(AlgorithmConstants::minRectangleEdgeSize_, AlgorithmConstants::maxRectangleEdgeSize_, recMaxSizeAltInput_));
	gridLayout_->addWidget(recMaxSizeAltInput_, 1, 2, Qt::AlignLeft);
	connect(recMaxSizeInput_, &QSlider::valueChanged, recMaxSizeAltInput_, [=]() {recMaxSizeAltInput_->setText(QString::number(recMaxSizeInput_->value())); });
	connect(recMaxSizeAltInput_, &QLineEdit::textChanged, recMaxSizeInput_, [=]() {recMaxSizeInput_->setValue(recMaxSizeAltInput_->text().toInt()); });

	boxSizeText_ = new QLabel();
	boxSizeText_->setFont(f);
	boxSizeText_->setText("Box Edge Length: ");
	gridLayout_->addWidget(boxSizeText_, 2, 0, Qt::AlignLeft);

	boxSizeSlider_ = new QSlider(Qt::Horizontal);
	boxSizeSlider_->setTickPosition(QSlider::TicksBelow);
	boxSizeSlider_->setTickInterval(1);
	boxSizeSlider_->setMaximum(AlgorithmConstants::maxBoxEdgeSize_);
	boxSizeSlider_->setMinimum(AlgorithmConstants::maxRectangleEdgeSize_);
	boxSizeSlider_->setValue(AlgorithmConstants::initialEdgeSize_);
	gridLayout_->addWidget(boxSizeSlider_, 2, 1, Qt::AlignLeft);

	boxSizeLineEdit_ = new QLineEdit();
	boxSizeLineEdit_->setFont(f);
	boxSizeLineEdit_->setText(QString::number(boxSizeSlider_->value()));
	boxSizeLineEdit_->setValidator(new QIntValidator(AlgorithmConstants::maxRectangleEdgeSize_, AlgorithmConstants::maxBoxEdgeSize_, boxSizeLineEdit_));
	gridLayout_->addWidget(boxSizeLineEdit_, 2, 2, Qt::AlignLeft);
	connect(boxSizeSlider_, &QSlider::valueChanged, boxSizeLineEdit_, [=]() {boxSizeLineEdit_->setText(QString::number(boxSizeSlider_->value())); });
	connect(boxSizeLineEdit_, &QLineEdit::textChanged, boxSizeSlider_, [=]() {boxSizeSlider_->setValue(boxSizeLineEdit_->text().toInt()); });

	
	connect(this->getRecAmountSlider(), &QSlider::valueChanged, this, &AlgorithmSelectionUI::ValueChangedI);
	connect(this->getRecAmountLineEdit(), &QLineEdit::textChanged, this, &AlgorithmSelectionUI::ValueChangedS);

	connect(this->getRecMaxSizeSlider(), &QSlider::valueChanged, this, &AlgorithmSelectionUI::ValueChangedI);
	connect(this->getRecMaxSizeLineEdit(), &QLineEdit::textChanged, this, &AlgorithmSelectionUI::ValueChangedS);

	connect(this->getBoxEdgeSlider(), &QSlider::valueChanged, this, &AlgorithmSelectionUI::ValueChangedI);
	connect(this->getBoxEdgeLineEdit(), &QLineEdit::textChanged, this, &AlgorithmSelectionUI::ValueChangedS);
	
}

AlgorithmSelectionUI::~AlgorithmSelectionUI()
{
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

	delete gridLayout_;
	gridLayout_ = nullptr;

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

void AlgorithmSelectionUI::ValueChangedS(QString dummy)
{
	emit OnValueChanged();
}

void AlgorithmSelectionUI::ValueChangedI(int dummy) {
	emit OnValueChanged();
}