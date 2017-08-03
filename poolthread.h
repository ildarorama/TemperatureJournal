#ifndef POOLTHREAD_H
#define POOLTHREAD_H

#include <QObject>
#include <QThread>
#include <QSqlDatabase>
#include <modbus/modbus.h>

class PoolThread : public QThread
{
    Q_OBJECT
public:
    PoolThread();
    void setPort(QString port);
protected:
    void run();
private:
    QString port;
    QSqlDatabase db;
    void createModbusMaster();
    int readData(uint16_t data[] );
    modbus_t *ctx = NULL;
    int connected =0;
signals:
    void newData(qint64 stmp,int t1,int t2);
};

#endif // POOLTHREAD_H
