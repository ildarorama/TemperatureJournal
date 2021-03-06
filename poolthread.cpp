#include "poolthread.h"
#include <QThread>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <modbus/modbus.h>

PoolThread::PoolThread() :
    port("/dev/ttyS0")
{

}

int PoolThread::readData(uint16_t data[]) {
    if ( ctx &&  connected ) {
        return modbus_read_registers(ctx, 0, 16, data);
    }
    return 0;
}

void PoolThread::createModbusMaster() {

    ctx = modbus_new_rtu(port.toStdString().c_str(), 9600, 'N', 8, 1);
    if (!ctx) {
        fprintf(stderr, "Failed to create the context: %s\n", modbus_strerror(errno));
        ctx=NULL;
    }
    if ( ! ctx ) return;

    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Unable to connect: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        connected=1;
    }

    //Set the Modbus address of the remote slave (to 3)
    modbus_set_slave(ctx, 3);


}

void PoolThread::run() {
    createModbusMaster();
    db = QSqlDatabase::addDatabase("QMYSQL","pool");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("cook");
    db.setUserName("cook");
    db.setPassword("cook");

    if ( db.open() ) {
        qInfo() << "Database successfuly opened in pooling thread";
    }

    QSqlQuery q(db);
    q.prepare("insert into test(t1,t2,stmp) values(:t1,:t2,:stmp)");

    while(! QThread::isInterruptionRequested()) {
        uint16_t data[16];
        if ( readData(data) == 16 ) {
            qDebug() << "Read successful";
            qint64 tt = QDateTime::currentMSecsSinceEpoch();
            newData(tt,data[0],data[1]);
            q.bindValue(":t1",data[0]);
            q.bindValue(":t2",data[1]);
            q.bindValue(":stmp",tt/1000);
            if (db.isOpen()) q.exec();
        } else {
             qDebug() << "Can not read Data";
        }
        QThread::sleep( 1);
        qDebug() << port;
    }

    if ( ctx ) {
        modbus_close(ctx);
        modbus_free(ctx);
    }
    qDebug() << "Close pooling thread";
}

void PoolThread::setPort(QString p) {
    port=p;
}

