#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QPainter>
#include <QList>
#include <QPoint>

enum GameState {
    WaitingToStart,
    Running,
    Paused,
    GameOver
};

enum Direction {
    Up,
    Down,
    Left,
    Right
};

class GameBoard : public QWidget
{
    Q_OBJECT

public:
    explicit GameBoard(int snakeSize = 20, int speed = 150, QWidget *parent = nullptr);

    void setSnakeSize(int size);
    void setSpeed(int ms);
    void startGame();
    void pauseGame();
    void resumeGame();
    void resetGame();

    GameState getState() const { return state; }
    int getScore() const { return score; }

signals:
    void scoreChanged(int score);
    void gameOverSignal();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void moveSnake();
    void generateFood();

    QList<QPoint> snake;
    Direction direction;
    Direction nextDirection;
    QPoint food;
    GameState state;

    int cellSize;       // size of each cell in pixels
    int speed;          // timer interval in ms
    int score;
    int cols;           // number of columns
    int rows;           // number of rows

    QTimer *timer;
};

#endif // GAMEBOARD_H