#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new PaintScene();   // Инициализируем графическую сцену
    ui->graphicsView->setScene(scene);
 //   ui->graphicsView->setRenderHint(QPainter::Antialiasing);                // Устанавливаем сглаживание
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Отключаем скроллбар по вертикали
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключаем скроллбар по горизонтали
  //  ui->graphicsView->setRenderHint(QPainter::Antialiasing);    // Настраиваем рендер
  //  ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground); // Кэш фона
    timer = new QTimer();       // Инициализируем таймер
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
    timer->start(100);          // Запускаем таймер
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotTimer()
{
    /* Переопределяем размеры графической сцены в зависимости
     * от размеров окна
     * */
    timer->stop();
    scene->setSceneRect(0,0, ui->graphicsView->width() - 20, ui->graphicsView->height() - 20);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    timer->start(100);
    QMainWindow::resizeEvent(event);
}



// Ромб
void MainWindow::on_pushButton_clicked()
{
    scene->setTypeCommand(PaintScene::EllipsType);
}

// Квадрат
void MainWindow::on_pushButton_2_clicked()
{
    scene->setTypeCommand(PaintScene::SquareType);
}

// Треугольник
void MainWindow::on_pushButton_3_clicked()
{
    scene->setTypeCommand(PaintScene::TriangleType);
}
// Перемещение
void MainWindow::on_pushButton_4_clicked()
{
    scene->setTypeCommand(PaintScene::Move);
}
//Связь
void MainWindow::on_pushButton_5_clicked()
{
    scene->setTypeCommand(PaintScene::Link);
}
void MainWindow::on_pushButton_6_clicked()
{
    scene->setTypeCommand(PaintScene::Del);
}
void MainWindow::on_pushButton_7_clicked()
{
    scene->setTypeCommand(PaintScene::Save);
}
void MainWindow::on_pushButton_8_clicked()
{
    scene->setTypeCommand(PaintScene::Load);
}



