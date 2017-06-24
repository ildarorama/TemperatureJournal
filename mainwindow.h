#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QLineSeries>
#include <QtSql>

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
    int z;
    QSqlDatabase db;
};

#endif // MAINWINDOW_H
