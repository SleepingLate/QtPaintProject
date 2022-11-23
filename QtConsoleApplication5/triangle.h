#pragma once

#include "figure.h"

class Triangle : public Figure
{
    Q_OBJECT

public:
    explicit Triangle(QObject *parent = 0);
    ~Triangle();

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

