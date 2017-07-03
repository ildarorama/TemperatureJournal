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




    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("cook");
    db.setUserName("cook");
    db.setPassword("cook");
    bool ok = db.open();


    ui->wdtChart->layout()->addWidget(createChart());

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
    z=0;
}


QChartView* MainWindow::createChart() {

    series = new QLineSeries();
    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle("Варка");

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(1);
    axisX->setFormat("dd:mm HH:MM:ss");
    axisX->setTitleText("Время");
    chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%d");
    axisY->setTitleText("Температура");
    chart->addAxis(axisY, Qt::AlignLeft);
    axisY->setRange(0,110);
    axisY->setTickCount(20);


    QDateTime min;
    QDateTime max;

    //axisX->setRange(min,max);



       for(int i=0;i<100;i++) {
           update();
       }

    series->attachAxis(axisX);
    series->attachAxis(axisY);
    chart->addSeries(series);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}

void MainWindow::update() {
    QDateTime mz;
    qint64 t=mz.currentDateTime().toMSecsSinceEpoch();

    series->append(t, z++);
    //series->append(t, z++);

    qInfo(QString::number(t).toLatin1().data());
    qInfo(QString::number(z).toLatin1().data());

    QString s="insert into test(i) values(";
    s.append(QString::number(z));
    s.append(")");
    qInfo(s.toLatin1().data());


    if ( db.isOpen() ) {
        db.exec(s);
        qInfo("Saved");
    }

  
}

MainWindow::~MainWindow()
{
    delete ui;
}
