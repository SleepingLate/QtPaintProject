#pragma once

#include "figure.h"

class Square : public Figure
{
    Q_OBJECT

public:
    explicit Square(QObject *parent = 0);
    ~Square();

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

