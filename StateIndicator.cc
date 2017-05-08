#include "StateIndicator.h"
#include "ui_StateIndicator.h"
#include <QPainter>

StateIndicator::StateIndicator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StateIndicator)
{
    ui->setupUi(this);
    this->color = Qt::red;
}

StateIndicator::~StateIndicator()
{
    delete ui;
}

void StateIndicator::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(Qt::gray);
    painter.drawEllipse(1, 1, this->width() - 2, this->height() - 2);
    painter.setPen(Qt::NoPen);
    painter.setBrush(this->color);
    painter.drawEllipse(3, 3, this->width() - 6, this->height() - 6);
}

void StateIndicator::setIndicatorColor(QColor color)
{
    this->color = color;
    this->update();
}
