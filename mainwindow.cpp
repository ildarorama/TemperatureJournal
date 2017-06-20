#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtSql>
#include <QTimer>


QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    series = new QLineSeries();

    QChart *chart = new QChart();
     chart->legend()->hide();
     chart->addSeries(series);
     //chart->createDefaultAxes();
     chart->setTitle("Simple line chart example");


     QDateTimeAxis *axisX = new QDateTimeAxis;
     axisX->setTickCount(10);
     axisX->setFormat("HH:MM:ss");
     axisX->setTitleText("Date");
     chart->addAxis(axisX, Qt::AlignBottom);
     series->attachAxis(axisX);

     QValueAxis *axisY = new QValueAxis;
        axisY->setLabelFormat("%d");
        axisY->setTitleText("Sunspots count");
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

     QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);


            setCentralWidget(chartView);

            QTimer *timer = new QTimer(this);
            connect(timer, SIGNAL(timeout()), this, SLOT(update()));
            timer->start(1000);

}
void MainWindow::update() {
    QDateTime mz;
    qint64 t=mz.currentDateTime().toMSecsSinceEpoch();
    series->append(t, z++);
    series->
    qInfo("printe time ");
}

MainWindow::~MainWindow()
{
    delete ui;
}
