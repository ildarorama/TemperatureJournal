#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QLineSeries>
#include <QtSql>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>

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
    void update();

private:
    Ui::MainWindow *ui;
    QtCharts::QLineSeries *series;
    double z;
    QSqlDatabase db;
    QtCharts::QChartView * createChart();
    QtCharts::QChart * chart;
    QtCharts::QChartView * chartView;
};

#endif // MAINWINDOW_H
