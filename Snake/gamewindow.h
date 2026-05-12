#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QStatusBar>
#include <QTimer>
#include "gameboard.h"
#include "gameconfigdialog.h"

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);

private:
    GameBoard *board;
    QLabel *scoreLabel;
    QLabel *statusLabel;

    void showConfigDialog();

private slots:
    void updateScore(int score);
    void handleGameOver();
};

#endif // GAMEWINDOW_H