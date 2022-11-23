#include "Ellips.h"

Ellips::Ellips( QObject *parent) :
    Figure(parent)
{
  //  Q_UNUSED(point)
}

Ellips::~Ellips()
{

}

// Реализуем метод отрисовки
void Ellips::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::black, 2));

    QRectF rect(endPoint().x() > startPoint().x() ? startPoint().x() : endPoint().x(),
        endPoint().y() > startPoint().y() ? startPoint().y() : endPoint().y(),
        qAbs(endPoint().x() - startPoint().x()),
        qAbs(endPoint().y() - startPoint().y()));

    painter->drawEllipse(rect);

    Q_UNUSED(option)
    Q_UNUSED(widget)
}
