#include "gameboard.h"
#include <QRandomGenerator>

GameBoard::GameBoard(int snakeSize, int speed, QWidget *parent)
    : QWidget(parent)
    , direction(Right)
    , nextDirection(Right)
    , state(WaitingToStart)
    , speed(speed)
    , score(0)
{
    setSnakeSize(snakeSize);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameBoard::moveSnake);

    setFocusPolicy(Qt::StrongFocus);

    // Initialize snake at center
    int startX = cols / 2;
    int startY = rows / 2;
    for (int i = 0; i < 3; i++) {
        snake.append(QPoint(startX - i, startY));
    }

    generateFood();
}

void GameBoard::setSnakeSize(int size)
{
    cellSize = size;
    // Fixed game area: 600x600
    cols = 600 / cellSize;
    rows = 600 / cellSize;
    setFixedSize(cols * cellSize, rows * cellSize);
}

void GameBoard::setSpeed(int ms)
{
    speed = ms;
    if (timer->isActive()) {
        timer->setInterval(speed);
    }
}

void GameBoard::startGame()
{
    if (state == WaitingToStart || state == GameOver) {
        resetGame();
        state = Running;
        timer->start(speed);
    }
}

void GameBoard::pauseGame()
{
    if (state == Running) {
        state = Paused;
        timer->stop();
        update();
    }
}

void GameBoard::resumeGame()
{
    if (state == Paused) {
        state = Running;
        timer->start(speed);
        update();
    }
}

void GameBoard::resetGame()
{
    snake.clear();
    direction = Right;
    nextDirection = Right;
    score = 0;

    int startX = cols / 2;
    int startY = rows / 2;
    for (int i = 0; i < 3; i++) {
        snake.append(QPoint(startX - i, startY));
    }

    generateFood();
    state = WaitingToStart;
    emit scoreChanged(score);
    update();
}

void GameBoard::moveSnake()
{
    direction = nextDirection;

    QPoint head = snake.first();
    switch (direction) {
    case Up:    head.ry() -= 1; break;
    case Down:  head.ry() += 1; break;
    case Left:  head.rx() -= 1; break;
    case Right: head.rx() += 1; break;
    }

    // Check wall collision
    if (head.x() < 0 || head.x() >= cols || head.y() < 0 || head.y() >= rows) {
        state = GameOver;
        timer->stop();
        update();
        emit gameOverSignal();
        return;
    }

    // Check self collision
    if (snake.contains(head)) {
        state = GameOver;
        timer->stop();
        update();
        emit gameOverSignal();
        return;
    }

    snake.prepend(head);

    // Check food
    if (head == food) {
        score += 10;
        emit scoreChanged(score);
        generateFood();
    } else {
        snake.removeLast();
    }

    update();
}

void GameBoard::generateFood()
{
    int attempts = 0;
    do {
        food.setX(QRandomGenerator::global()->bounded(cols));
        food.setY(QRandomGenerator::global()->bounded(rows));
        attempts++;
    } while (snake.contains(food) && attempts < 1000);
}

void GameBoard::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        if (direction != Down) nextDirection = Up;
        break;
    case Qt::Key_Down:
        if (direction != Up) nextDirection = Down;
        break;
    case Qt::Key_Left:
        if (direction != Right) nextDirection = Left;
        break;
    case Qt::Key_Right:
        if (direction != Left) nextDirection = Right;
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (state == WaitingToStart || state == GameOver) {
            startGame();
        } else if (state == Paused) {
            resumeGame();
        }
        break;
    case Qt::Key_Space:
        if (state == Running) {
            pauseGame();
        } else if (state == Paused) {
            resumeGame();
        }
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void GameBoard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    // Black background
    painter.fillRect(rect(), Qt::black);

    // Draw border (right and bottom gray edges, like the screenshot)
    painter.setPen(QPen(QColor(160, 160, 160), 2));
    painter.drawLine(rect().right(), rect().top(), rect().right(), rect().bottom());
    painter.drawLine(rect().left(), rect().bottom(), rect().right(), rect().bottom());

    // Draw grid lines (subtle)
    painter.setPen(QPen(QColor(30, 30, 30), 1));
    for (int x = 0; x <= cols; x++) {
        painter.drawLine(x * cellSize, 0, x * cellSize, rows * cellSize);
    }
    for (int y = 0; y <= rows; y++) {
        painter.drawLine(0, y * cellSize, cols * cellSize, y * cellSize);
    }

    // Draw snake body (white squares with small gaps)
    int gap = cellSize / 6; // small gap between segments
    for (int i = 0; i < snake.size(); i++) {
        QPoint segment = snake[i];
        int x = segment.x() * cellSize + gap;
        int y = segment.y() * cellSize + gap;
        int w = cellSize - 2 * gap;
        int h = cellSize - 2 * gap;

        if (i == 0) {
            // Snake head - slightly different shade
            painter.fillRect(x, y, w, h, QColor(240, 240, 240));
        } else {
            // Snake body - white
            painter.fillRect(x, y, w, h, Qt::white);
        }
    }

    // Draw food (red square)
    int foodGap = cellSize / 6;
    painter.fillRect(
        food.x() * cellSize + foodGap,
        food.y() * cellSize + foodGap,
        cellSize - 2 * foodGap,
        cellSize - 2 * foodGap,
        Qt::red
    );

    // Overlay text for game states
    if (state == WaitingToStart) {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 20, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "按回车键开始游戏");
    } else if (state == Paused) {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 20, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "游戏暂停\n按空格键继续");
    } else if (state == GameOver) {
        painter.setPen(Qt::red);
        painter.setFont(QFont("Arial", 20, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter,
            "游戏结束!\n得分: " + QString::number(score) + "\n按回车键重新开始");
    }
}