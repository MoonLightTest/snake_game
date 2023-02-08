#include "game.h"

GameField::GameField()
{
    setFixedSize(300, 300);
    setFocusPolicy(Qt::StrongFocus);
    m_snakeItemSize = 10;
    m_fieldSize= width() / m_snakeItemSize;
    StartNewGame();
}

void GameField::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QBrush gameFieldBrush(QColor(139, 144, 163), Qt::SolidPattern);
    QBrush snakeBrush(QColor(144, 206, 147), Qt::SolidPattern);
    QBrush foodBrush(QColor(247, 103, 123), Qt::SolidPattern);
    QPainter painter;
    painter.begin(this);
    if (m_isGameOver)
    {
        painter.setPen(QColor(250, 250, 250));
        painter.setFont(QFont("Arial", 15, 700));
        painter.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, "ИГРА ПРОИГРАНА\nсчет " + QString::number(m_score));
        return;
    }
    // отрисовка игрового поля
    painter.setBrush(gameFieldBrush);
    painter.setPen(QColor(50, 50, 50));
    painter.drawRect(0, 0, width()-1, height()-1);
    // отрисовка змейки
    painter.setBrush(snakeBrush);
    painter.setPen(QColor(0, 0, 0));
    for (int i = 0; i < m_snake->m_snakeBody.size(); i++)
    {
        painter.drawEllipse(m_snake->m_snakeBody[i]->m_x * m_snakeItemSize,
                            m_snake->m_snakeBody[i]->m_y * m_snakeItemSize,
                            m_snakeItemSize,
                            m_snakeItemSize);
    }

    // отрисовка еды
    painter.setBrush(foodBrush);
    painter.drawEllipse(m_food->m_x * m_snakeItemSize,
                        m_food->m_y * m_snakeItemSize,
                        m_snakeItemSize,
                        m_snakeItemSize);



    painter.end();
    m_isMoveBlocked = false;
}

void GameField::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Space)
    {
        if (m_isGameOver)
        {
            StartNewGame();
            return;
        }
        m_isPause = !m_isPause;
        SetGameStatus();
    }
    if (m_isMoveBlocked)
    {
        return;
    }
    if (e->key() == Qt::Key_Left && m_snake->m_snakeDirection != Snake::SnakeDirection::right)
    {
        m_snake->m_snakeDirection = Snake::SnakeDirection::left;
    }
    if (e->key() == Qt::Key_Right && m_snake->m_snakeDirection != Snake::SnakeDirection::left)
    {
        m_snake->m_snakeDirection = Snake::SnakeDirection::right;
    }
    if (e->key() == Qt::Key_Down && m_snake->m_snakeDirection != Snake::SnakeDirection::up)
    {
        m_snake->m_snakeDirection = Snake::SnakeDirection::down;
    }
    if (e->key() == Qt::Key_Up && m_snake->m_snakeDirection != Snake::SnakeDirection::down)
    {
        m_snake->m_snakeDirection = Snake::SnakeDirection::up;
    }
    m_isMoveBlocked = true;
}

void GameField::SetGameStatus()
{
    QString text;
    if (m_isPause)
    {
        m_moveSnakeTimer->stop();
        text = "счет : " + QString::number(m_score) + "\nпродолжить - ПРОБЕЛ";
    }
    else
    {
        m_moveSnakeTimer->start(100);
        text = "счет : " + QString::number(m_score) + "\nпауза - ПРОБЕЛ";
    }
    emit ChangeTextSignal(text);
}

void GameField::GameOver()
{
    m_isGameOver = true;
    QString text = "вы проиграли, ваш счет " + QString::number(m_score) + "\nначать заново - ПРОБЕЛ";
    emit ChangeTextSignal(text);
    m_moveSnakeTimer->stop();
    delete m_snake;
    delete m_moveSnakeTimer;
    delete m_food;
}

void GameField::StartNewGame()
{
    m_isPause = false;
    m_isMoveBlocked = false;
    m_isGameOver = false;
    m_score = 0;
    m_snake = new Snake();
    m_food = new SnakeItem(m_fieldSize/2, m_fieldSize /2);
    m_moveSnakeTimer = new QTimer();
    connect(m_moveSnakeTimer, &QTimer::timeout, this, &GameField::MoveSnakeSlot);
    m_moveSnakeTimer->start(100);
    QString text = "счет : " + QString::number(m_score) + "\nпауза - ПРОБЕЛ";
    emit ChangeTextSignal(text);
}

void GameField::CreateFood()
{
    m_food->m_x = QRandomGenerator::global()->bounded(0, m_fieldSize - 1);
    m_food->m_y = QRandomGenerator::global()->bounded(0, m_fieldSize - 1);
    for (int i = 0; i < m_snake->m_snakeBody.size(); i++)
    {
        if ((m_food->m_x == m_snake->m_snakeBody[i]->m_x) && (m_food->m_y == m_snake->m_snakeBody[i]->m_y))
        {
            return CreateFood();
        }
    }
}

void GameField::MoveSnakeSlot(){
    SnakeItem *newSnakeItem;
    if (m_snake->m_snakeDirection == Snake::SnakeDirection::right)
    {
        newSnakeItem = new SnakeItem(m_snake->m_snakeBody[0]->m_x + 1, m_snake->m_snakeBody[0]->m_y);
    }
    else if (m_snake->m_snakeDirection == Snake::SnakeDirection::left)
    {
        newSnakeItem = new SnakeItem(m_snake->m_snakeBody[0]->m_x - 1, m_snake->m_snakeBody[0]->m_y);
    }
    else if (m_snake->m_snakeDirection == Snake::SnakeDirection::up)
    {
        newSnakeItem = new SnakeItem(m_snake->m_snakeBody[0]->m_x, m_snake->m_snakeBody[0]->m_y - 1);
    }
    else
    {
        newSnakeItem = new SnakeItem(m_snake->m_snakeBody[0]->m_x, m_snake->m_snakeBody[0]->m_y + 1);
    }

    if (newSnakeItem->m_x >= m_fieldSize)
    {
        newSnakeItem->m_x = 0;
    }
    else if (newSnakeItem->m_x < 0)
    {
        newSnakeItem->m_x = m_fieldSize - 1;
    }
    else if (newSnakeItem->m_y >= m_fieldSize)
    {
        newSnakeItem->m_y = 0;
    }
    else if (newSnakeItem->m_y < 0)
    {
        newSnakeItem->m_y = m_fieldSize - 1;;
    }
    // проигрыш
    for (int i = 1; i < m_snake->m_snakeBody.size(); i++){
        if ((m_snake->m_snakeBody[0]->m_x == m_snake->m_snakeBody[i]->m_x) && (m_snake->m_snakeBody[0]->m_y == m_snake->m_snakeBody[i]->m_y))
        {
            GameOver();
        }
    }
    // получение еды
    if ((newSnakeItem->m_x == m_food->m_x) && (newSnakeItem->m_y == m_food->m_y))
    {
        m_score++;
        CreateFood();
        QString text = "счет : " + QString::number(m_score) + "\nпауза - ПРОБЕЛ";
        emit ChangeTextSignal(text);
    }
    else
    {
        m_snake->m_snakeBody.removeLast();
    }
    m_snake->m_snakeBody.insert(0, newSnakeItem);
    repaint();
}

SnakeItem::SnakeItem(int x, int y) : m_x(x), m_y(y)
{

}

Snake::Snake()
{
    m_snakeBeginSize = 4;
    for (int i = 0; i < m_snakeBeginSize; i++)
    {
        m_snakeBody.insert(0, new SnakeItem(i, 0));
    }
    m_snakeDirection = SnakeDirection::right;
}
