#ifndef STATEINDICATOR_H
#define STATEINDICATOR_H

#include <QWidget>

namespace Ui {
class StateIndicator;
}

class StateIndicator : public QWidget
{
    Q_OBJECT
    QColor color;
public:
    explicit StateIndicator(QWidget *parent = 0);
    ~StateIndicator();
    void setIndicatorColor(QColor);

private:
    Ui::StateIndicator *ui;
    void paintEvent(QPaintEvent*);
};


#endif // STATEINDICATOR_H
