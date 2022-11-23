#include "figure.h"

Figure::Figure(QObject *parent) :
    QGraphicsObject()
{
    // Устанавливаем стартовую координату для отрисовки фигуры
    //this->setStartPoint(point);
  //  this->setEndPoint(point);

}

Figure::~Figure()
{
    QList<QGraphicsLineItem*>().swap(lines);
}

QRectF Figure::boundingRect() const 
{
    /* Возвращаем область, в которой лежит фигура.
     * Обновляемая область зависит от стартовой точки отрисовки и от конечной точки
     * */
    return QRectF((endPoint().x() > startPoint().x() ? startPoint().x() : endPoint().x()) ,
                  (endPoint().y() > startPoint().y() ? startPoint().y() : endPoint().y()) ,
                  qAbs(endPoint().x() - startPoint().x()) ,
                  qAbs(endPoint().y() - startPoint().y()) );
}

void Figure::setStartPoint(const QPointF point)
{
    m_startPoint = point;
}

void Figure::setEndPoint(const QPointF point)
{
    m_endPoint = point;
}

QPointF Figure::startPoint() const
{
    return m_startPoint;
}

QPointF Figure::endPoint() const
{
    return m_endPoint;
}


QGraphicsLineItem* Figure::lineTo(Figure* targetItem) {
    //рисуем линию между двумя итемами
    Figure* pressedItem = this;
    QPointF dp1 = pressedItem->boundingRect().center();
    QPointF dp2 = targetItem->boundingRect().center();
    QPointF p1 = pressedItem->scenePos() + dp1; //позиции стартового и целевого итемов
    QPointF p2 = targetItem->scenePos() + dp2;
    QLineF pline = QLineF(p1, p2); //вероятная линия между ними

    if (pressedItem->checkLines(pline)) {
        //если не дублируем линию-создаем ее
        QGraphicsLineItem* line = new QGraphicsLineItem;
        line->setLine(pline);
        pressedItem->lines.append(line); //добавляем линию стартовому и целевому итемам
        targetItem->lines.append(line);
        return line;
    }
    else
        return nullptr; //если линия дублируется возвращаем нуль
}


bool Figure::checkLines(QLineF testLine) {
    //не рисуем ли линию дважды?
    for (const auto line : lines) {

        QLineF normalLine = line->line(); //линия от итема
        QPointF p1 = normalLine.p1();
        QPointF p2 = normalLine.p2();
        QLineF invertedLine = QLineF(p2, p1); //линия к итему

        if (normalLine == testLine || invertedLine == testLine)
            return false;
    }
    return true;
}

void Figure::redrawLines() {

    QPointF dp = this->boundingRect().center();
    QPointF itemPos = this->scenePos() + dp;

    for(auto& line : lines) {

        QPointF lineP2 = line->line().p2();
        line->setLine(QLineF(itemPos, lineP2)); //отрисовка всех линий итема
    }
}

void Figure::adjustLines() { //подгонка линий итема

    QPointF dp = this->boundingRect().center();
    QPointF itemPos = this->scenePos() + dp;

    for(auto& line : lines) {
        //перебираем все линии итема
        QPointF lineP1 = line->line().p1(); //стартовая точка
        QPointF lineP2 = line->line().p2(); //конечная точка

        if (lineP2 == itemPos) {
            line->setLine(QLineF(lineP2, lineP1)); //если конечная точка совпадает с итемом-переворачиваем
        }
        else {
            line->setLine(QLineF(lineP1, lineP2)); //иначе оставляем как есть
        }
    }
}
