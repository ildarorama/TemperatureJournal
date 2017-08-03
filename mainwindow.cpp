#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtSql>
#include <QTimer>
#include <QSettings>



QT_CHARTS_USE_NAMESPACE


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    setting("setting.cfg",QSettings::NativeFormat)
{
    ui->setupUi(this);


    //QSettings setting=QSettings("setting.cfg",QSettings::NativeFormat);
    QString sPort=setting.value("port","/dev/ttyS0").toString();
    qint32 iPool=setting.value("pool","1000").toInt();
    ui->edtPort->setText(sPort);
    ui->edtPool->setValue(iPool);

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("cook");
    db.setUserName("cook");
    db.setPassword("cook");
    bool ok = db.open();

    ui->wdtChart->layout()->addWidget(createChart());


    QDateTime from=QDateTime::currentDateTime();
    ui->edtDateFrom->setDateTime(from);
    ui->edtDateTo->setDateTime(from.addDays(1));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
    z=0;

    thread.setPort(ui->edtPort->toPlainText());
    QObject::connect(&thread,&PoolThread::newData,this,&MainWindow::update);
    thread.start();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if ( thread.isRunning() ) {
        thread.terminate();
        thread.wait(1000);
    }
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


void MainWindow::update(qint64 stmp,int t1,int t2) {

    qint64 q=QDateTime::currentMSecsSinceEpoch();
    series->append(stmp,t1/10.0);
    series2->append(stmp,t2/10.0);
    chart->scroll(1,0);
    chartView->repaint();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    if (HchartView) { ui->wdtHistory->layout()->removeWidget(HchartView);HchartView->deleteLater(); HchartView=NULL; }
    if (Hchart) { Hchart->deleteLater(); Hchart=NULL; }
    if (Hseries) { Hseries->deleteLater(); Hseries=NULL; }
    if (Hseries2) { Hseries2->deleteLater(); Hseries2=NULL; }

    QDateTime from=ui->edtDateFrom->dateTime();
    QDateTime to=ui->edtDateTo->dateTime();
    QString s="select stmp, t1, t2 from test where stmp>";
    s.append(QString::number( from.toSecsSinceEpoch()));
    s.append(" and stmp < ");
    s.append(QString::number( to.toSecsSinceEpoch()));

    QSqlQuery q=db.exec(s);
    Hseries = new QSplineSeries();
    Hseries2 = new QSplineSeries();

    Hseries2->setName("Емкость 2");
    Hseries->setName("Емкость 1");

    while(q.next()) {
        qint64 stmp=q.value(0).toInt()*1000;
        qint32 t1=q.value(1).toInt();
        qint32 t2=q.value(2).toInt();
        Hseries->append(stmp,t1/10.0);
        Hseries2->append(stmp,t2/10.0);
    }
    Hchart = new QChart();

    Hchart->setTitle("Варка перчиков");
    Hchart->addSeries(Hseries);
    Hchart->addSeries(Hseries2);

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("hh:mm:ss");
    axisX->setTitleText("Время");
    Hchart->addAxis(axisX, Qt::AlignBottom);
    Hseries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Температура");
    axisY->setRange(0,110);
    Hchart->addAxis(axisY, Qt::AlignLeft);
    Hseries->attachAxis(axisY);


    axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Температура");
    axisY->setRange(0,110);
    axisY->hide();
    Hchart->addAxis(axisY, Qt::AlignLeft);
    Hseries2->attachAxis(axisY);

    HchartView = new QChartView(Hchart);
    HchartView->setRenderHint(QPainter::Antialiasing);
    ui->wdtHistory->layout()->addWidget(HchartView);

}

void MainWindow::on_pushButton_2_clicked()
{
    setting.setValue("port",ui->edtPort->toPlainText());
    setting.setValue("pool",ui->edtPool->value());
}
