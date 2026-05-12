#ifndef GAMECONFIGDIALOG_H
#define GAMECONFIGDIALOG_H

#include <QDialog>
#include <QRadioButton>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

enum Difficulty {
    Easy,
    Medium,
    Hard,
    VeryHard
};

class GameConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameConfigDialog(QWidget *parent = nullptr);

    int getSnakeSize() const;
    Difficulty getDifficulty() const;

private:
    QSpinBox *snakeSizeSpinBox;
    QRadioButton *easyRadio;
    QRadioButton *mediumRadio;
    QRadioButton *hardRadio;
    QRadioButton *veryHardRadio;
    QPushButton *startButton;
};

#endif // GAMECONFIGDIALOG_H