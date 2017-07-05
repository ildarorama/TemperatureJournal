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
    chart->addSeries(series);
    chart->setTitle("Варка");

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("hh mm ss");
    axisX->setTitleText("Время");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Температура");
    //axisY->setRange(0,110);
    axisY->setTickCount(20);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QPoint p;
    p.setX(QDateTime::currentMSecsSinceEpoch());
    p.setY(10.0);
    series->insert(0,p);



    chartView = new QChartView(chart);


    chartView->setRenderHint(QPainter::Antialiasing);


    return chartView;
}

void MainWindow::update() {

    series->append(QDateTime::currentMSecsSinceEpoch(),z);
    z=z+5;
    //series->append(, z++);

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
