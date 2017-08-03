#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QLineSeries>
#include <QtSql>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include "poolthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void update(qint64 stmp,int t1,int t2);


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QtCharts::QLineSeries *series;
    QtCharts::QLineSeries *series2;
    QtCharts::QLineSeries *Hseries=NULL;
    QtCharts::QLineSeries *Hseries2=NULL;
    QSettings setting;
    double z;
    QSqlDatabase db;
    QtCharts::QChartView * createChart();
    QtCharts::QChart * chart;
    QtCharts::QChartView * chartView;
    QtCharts::QChart * Hchart=NULL;
    QtCharts::QChartView * HchartView=NULL;
    PoolThread thread;

    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
