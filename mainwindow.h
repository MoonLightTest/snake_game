#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game.h"
#include "helpfield.h"
#include <QMainWindow>
#include <QGridLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    GameField *m_gameField;
    HelpField *m_helpField;
    QWidget *m_centralWidget;
    QGridLayout *m_gridLayout;
};
#endif // MAINWINDOW_H
