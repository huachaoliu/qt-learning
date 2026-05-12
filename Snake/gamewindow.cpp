#include "gamewindow.h"

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Show configuration dialog first
    GameConfigDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) {
        // If user cancels, close the app
        QTimer::singleShot(0, this, &QWidget::close);
        return;
    }

    int snakeSize = dialog.getSnakeSize();
    Difficulty difficulty = dialog.getDifficulty();

    // Map difficulty to speed
    int speed = 150;
    switch (difficulty) {
    case Easy:      speed = 200; break;
    case Medium:    speed = 150; break;
    case Hard:      speed = 100; break;
    case VeryHard:  speed = 60;  break;
    }

    board = new GameBoard(snakeSize, speed, this);
    setCentralWidget(board);

    setWindowTitle("Snake 贪吃蛇");
    setStyleSheet("QMainWindow { background-color: black; }");

    // Score label in status bar
    scoreLabel = new QLabel("得分: 0");
    scoreLabel->setStyleSheet("color: white; font-size: 14px; padding: 4px;");
    statusBar()->setStyleSheet("QStatusBar { background-color: #333; }");
    statusBar()->addWidget(scoreLabel, 1);

    statusLabel = new QLabel("按回车键开始游戏");
    statusLabel->setStyleSheet("color: #aaa; font-size: 12px; padding: 4px;");
    statusBar()->addPermanentWidget(statusLabel);

    connect(board, &GameBoard::scoreChanged, this, &GameWindow::updateScore);
    connect(board, &GameBoard::gameOverSignal, this, &GameWindow::handleGameOver);

    board->setFocus();
}

void GameWindow::updateScore(int score)
{
    scoreLabel->setText("得分: " + QString::number(score));
}

void GameWindow::handleGameOver()
{
    statusLabel->setText("游戏结束! 按回车键重新开始");
}

void GameWindow::showConfigDialog()
{
    GameConfigDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        board->setSnakeSize(dialog.getSnakeSize());

        Difficulty difficulty = dialog.getDifficulty();
        int speed = 150;
        switch (difficulty) {
        case Easy:      speed = 200; break;
        case Medium:    speed = 150; break;
        case Hard:      speed = 100; break;
        case VeryHard:  speed = 60;  break;
        }
        board->setSpeed(speed);
        board->resetGame();
        board->setFocus();
    }
}