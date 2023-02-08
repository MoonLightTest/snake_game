#ifndef HELPFIELD_H
#define HELPFIELD_H

#include <QWidget>
#include <QPainter>

class HelpField : public QWidget
{
public:
    HelpField();
protected:
    void paintEvent(QPaintEvent *e) override;

private:
    QString m_text;

public slots:
    void ChangeTextSlot(QString text);
};

#endif // HELPFIELD_H
