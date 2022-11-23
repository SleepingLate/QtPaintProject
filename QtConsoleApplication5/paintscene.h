#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QFileDialog>
#include <QKeyEvent>
#include "figure.h"

struct FigureData
{
public:
    int id;
    int type;	//0-квадрат 1-элипс 2-треугольник
    int cordx;
    int cordy;
    int centrx;
    int centry;
};

struct LineData
{
    int id1;
    int id2;

};

class PaintScene : public QGraphicsScene
{
    Q_OBJECT
    // Свойство текущего типа используемой фигуры
    Q_PROPERTY(int typeCommand
               READ typeCommand WRITE setTypeCommand
               NOTIFY typeCommandChanged)

public:
    explicit PaintScene(QObject *parent = 0);
    ~PaintScene();

    int typeCommand() const;                 // Возвращение текщего типа
    void setTypeCommand(const int type);     // Установка текущего типа
 
    // Перечисление типов используемых фигур
    enum FigureTypes {
        SquareType,
        EllipsType,
        TriangleType,
        Move,
        Link,
        Del,
        Save,
        Load
    };

signals:
    void typeCommandChanged();               // Сигнал об изменении типа текущей фигуры


private:
    QVector<Figure*> saveptrfigure;         // Вектор с указатилями на все существующие фигуры
    QVector<FigureData> savelist;
    QVector<LineData> savelistline;
    Figure* tempFigure;  // Объект для временного хранения рисуемой фигуры
    Figure* pressedItem;
    Figure* targedItem;
    QGraphicsLineItem* Line = new QGraphicsLineItem;
    QPointF p1;          //стартовая точка линии
    QPointF p2;          //стартовая точка линии
    int m_typeCommand = 0;   // текущий тип фигуры
    QPointF m_shiftMouseCoords; //дельта сдвига мышки
    int count = 0;
    QString GlobalPath;
    QFile file;
    bool flagescape = false;
    bool flagrightclick = false;

    // Для рисования используем события мыши

    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void keyPressEvent(QKeyEvent* e);
    void savePtr(Figure* figure);
    void writetolist();
    void figuretojson();
    void linetojson();
    void savetojson();
    void loadtojson();
    void clearScene();
    void figurefromjson();
    void linefromjson();
    void deleteFigure(Figure* figure);
    void paintloaddata();
};

