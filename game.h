#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QPainter>
#include <QList>
#include <QTimer>
#include <QKeyEvent>
#include <QRandomGenerator>

class SnakeItem;
class Snake;
class GameField;

class SnakeItem
{
    friend Snake;
    friend GameField;
public:
    SnakeItem(int x, int y);
private:
    int m_x;
    int m_y;
};

class Snake
{
    friend GameField;
public:
    Snake();
private:
    QList<SnakeItem*> m_snakeBody;
    int m_snakeBeginSize;
    enum SnakeDirection
    {
        up,
        left,
        down,
        right
    };
    SnakeDirection m_snakeDirection;
};

class GameField : public QWidget
{
    Q_OBJECT
public:
    GameField();

protected:
    void paintEvent(QPaintEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

private:
    Snake *m_snake;
    SnakeItem *m_food;
    int m_snakeItemSize;
    QTimer *m_moveSnakeTimer;
    int m_fieldSize;
    int m_score;
    bool m_isPause;
    bool m_isMoveBlocked;
    bool m_isGameOver;
    void SetGameStatus();
    void GameOver();
    void StartNewGame();
    void CreateFood();

private slots:
    void MoveSnakeSlot();

signals:
    void ChangeTextSignal(QString text);
};

#endif // GAME_H
