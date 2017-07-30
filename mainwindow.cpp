#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
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


    //qint64 q=QDateTime::currentMSecsSinceEpoch();
    QDateTime to=QDateTime::currentDateTime();
    QDateTime from=to;
    to.addDays(1);
    ui->edtDateFrom->setDateTime(from);
    ui->edtDateTo->setDateTime(to);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
    z=0;
}


QChartView* MainWindow::createChart() {

    series = new QSplineSeries();
    series2 = new QSplineSeries();

    series2->setName("Емкость 2");
    series->setName("Емкость 1");


    qint64 tt=QDateTime::currentMSecsSinceEpoch()-300000;
    for(int i=0;i<300;i++) {
        series->append(tt+(i*1000),0);
        series2->append(tt+(i*1000),0);
    }

    chart = new QChart();
    //chart->legend()->hide();
    chart->setTitle("Варка перчиков");
    chart->addSeries(series);
    chart->addSeries(series2);

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("hh:mm:ss");
    axisX->setTitleText("Время");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Температура");
    axisY->setRange(0,110);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);


    axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Температура");
    axisY->setRange(0,110);
    axisY->hide();
    chart->addAxis(axisY, Qt::AlignLeft);
    series2->attachAxis(axisY);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}

int idx=0;

void MainWindow::showHistory() {

}
void MainWindow::update() {

    qint64 q=QDateTime::currentMSecsSinceEpoch();
    series->append(q,qSin(qDegreesToRadians(z))*110);
    series2->append(q,qSin(qDegreesToRadians(z+30))*110);
    z=z+1;
    if (z>360) {
        z=1;
    }

    QString s="insert into test(i) values(";
    s.append(QString::number(z));
    s.append(")");
    qInfo(s.toLatin1().data());
    if ( db.isOpen() ) {
        db.exec(s);
        qInfo("Saved");
    }
    chart->scroll(1,0);
    chartView->repaint();

}

MainWindow::~MainWindow()
{
    delete ui;
}
