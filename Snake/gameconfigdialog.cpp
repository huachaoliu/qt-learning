#include "gameconfigdialog.h"

GameConfigDialog::GameConfigDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("游戏说明");
    setFixedSize(420, 320);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    // Title
    QLabel *titleLabel = new QLabel("游戏说明");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);

    // Controls section
    QHBoxLayout *controlsLayout = new QHBoxLayout();

    // Direction arrow icons
    QVBoxLayout *arrowsLayout = new QVBoxLayout();
    QHBoxLayout *arrowRow = new QHBoxLayout();

    // Create arrow direction squares
    QString arrowStyle = "QPushButton {"
                         "  background-color: #4A90D9;"
                         "  color: white;"
                         "  border: none;"
                         "  font-size: 16px;"
                         "  font-weight: bold;"
                         "  min-width: 36px;"
                         "  min-height: 36px;"
                         "  max-width: 36px;"
                         "  max-height: 36px;"
                         "}";

    QPushButton *leftBtn = new QPushButton("←");
    QPushButton *upBtn = new QPushButton("↑");
    QPushButton *rightBtn = new QPushButton("→");
    QPushButton *downBtn = new QPushButton("↓");

    leftBtn->setStyleSheet(arrowStyle);
    upBtn->setStyleSheet(arrowStyle);
    rightBtn->setStyleSheet(arrowStyle);
    downBtn->setStyleSheet(arrowStyle);

    // These buttons are just for display, disable them
    leftBtn->setEnabled(false);
    upBtn->setEnabled(false);
    rightBtn->setEnabled(false);
    downBtn->setEnabled(false);

    arrowRow->addWidget(leftBtn);
    arrowRow->addWidget(upBtn);
    arrowRow->addWidget(rightBtn);
    arrowRow->addWidget(downBtn);
    arrowsLayout->addLayout(arrowRow);
    controlsLayout->addLayout(arrowsLayout);

    QLabel *controlsLabel = new QLabel("控制蛇的方向，回车键启动游戏，\n空格键暂停游戏");
    controlsLabel->setWordWrap(true);
    controlsLabel->setStyleSheet("font-size: 12px;");
    controlsLayout->addWidget(controlsLabel);
    controlsLayout->addStretch();

    mainLayout->addLayout(controlsLayout);

    // Snake size section
    QHBoxLayout *sizeLayout = new QHBoxLayout();
    QLabel *sizeLabel = new QLabel("蛇的大小:");
    sizeLabel->setStyleSheet("font-size: 13px;");

    snakeSizeSpinBox = new QSpinBox();
    snakeSizeSpinBox->setRange(5, 50);
    snakeSizeSpinBox->setValue(20);
    snakeSizeSpinBox->setStyleSheet(
        "QSpinBox {"
        "  background-color: #90EE90;"
        "  border: 1px solid #666;"
        "  min-width: 80px;"
        "  min-height: 28px;"
        "  font-size: 13px;"
        "}"
    );

    QLabel *sizeHint = new QLabel("(在15到50之间最佳)");
    sizeHint->setStyleSheet("font-size: 11px; color: #888;");

    sizeLayout->addWidget(sizeLabel);
    sizeLayout->addWidget(snakeSizeSpinBox);
    sizeLayout->addWidget(sizeHint);
    sizeLayout->addStretch();
    mainLayout->addLayout(sizeLayout);

    // Difficulty section
    QHBoxLayout *diffLayout = new QHBoxLayout();
    QLabel *diffLabel = new QLabel("难度:");
    diffLabel->setStyleSheet("font-size: 13px;");

    easyRadio = new QRadioButton("简单");
    mediumRadio = new QRadioButton("中等");
    hardRadio = new QRadioButton("困难");
    veryHardRadio = new QRadioButton("非常困难");

    mediumRadio->setChecked(true); // default medium

    diffLayout->addWidget(diffLabel);
    diffLayout->addWidget(easyRadio);
    diffLayout->addWidget(mediumRadio);
    diffLayout->addWidget(hardRadio);
    diffLayout->addWidget(veryHardRadio);
    diffLayout->addStretch();
    mainLayout->addLayout(diffLayout);

    // Start button
    startButton = new QPushButton("开始游戏");
    startButton->setStyleSheet(
        "QPushButton {"
        "  background-color: white;"
        "  border: 2px solid black;"
        "  font-size: 14px;"
        "  font-weight: bold;"
        "  min-width: 140px;"
        "  min-height: 40px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #f0f0f0;"
        "}"
    );
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(startButton);
    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);

    connect(startButton, &QPushButton::clicked, this, &QDialog::accept);
}

int GameConfigDialog::getSnakeSize() const
{
    return snakeSizeSpinBox->value();
}

Difficulty GameConfigDialog::getDifficulty() const
{
    if (easyRadio->isChecked()) return Easy;
    if (mediumRadio->isChecked()) return Medium;
    if (hardRadio->isChecked()) return Hard;
    return VeryHard;
}