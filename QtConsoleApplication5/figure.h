#pragma once

#include <QGraphicsObject>
#include <QPainter>

class Figure : public QGraphicsObject
{
    Q_OBJECT
    // Свойство стартовой точки, относительно которой отрисовываем фигуру
    Q_PROPERTY(QPointF startPoint
               READ startPoint WRITE setStartPoint
               NOTIFY pointChanged)
    // Свойство конечно точки, до куда отрисовываем фигуру
    Q_PROPERTY(QPointF endPoint
               READ endPoint WRITE setEndPoint
               NOTIFY pointChanged)
public:
    explicit Figure(QObject *parent = 0);
    ~Figure();

    QPointF startPoint() const; // Стартовая точка
    QPointF endPoint() const;   // Конечная точка
    void setStartPoint(const QPointF point);    // Установка стартовой точки
    void setEndPoint(const QPointF point);      // Установка конечной точки

    QRectF boundingRect() const override;    // Область, в которой содержится фигура
    QList<QGraphicsLineItem*>  lines; //список принадлжащих линий
    QGraphicsLineItem* lineTo(Figure* targetItem);
    void adjustLines();
    void redrawLines();
signals:
    void pointChanged();    // Сигнал об изменении точки

private:
    bool checkLines(QLineF testLine);
    QPointF m_startPoint;   // Стартовая точка
    QPointF m_endPoint;     // Конечная точка


//public slots:
//    void updateRomb();     // Слот обновления области, в которой содержится фигура
};

