#pragma once

#include "figure.h"

/* Наследуемся от класса Figure,
 * в котором реализован общий для всех фигур функционал
 * */
class Ellips : public Figure
{
    Q_OBJECT

public:
    explicit Ellips(QObject *parent = 0);
    ~Ellips();

private:
    // Для Ромба реализуем только саму отрисовку
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};
