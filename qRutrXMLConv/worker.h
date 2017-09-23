#ifndef WORKER_H
#define WORKER_H
#include <QThread>
#include <QFile>
#include <QXmlStreamReader>
#include <QMap>

#include "database.h"

class Worker : public QThread
{
    Q_OBJECT
public:
    Worker(DataBase* idb, QFile* iXMLfile);
    ~Worker();
private:
    DataBase* m_db;
    QFile* m_XMLfile;
    bool m_stop = false;
    int m_count = 0;

protected:
    void run();

signals:
    void signalAppendToLog(QString str);

public slots:
    void slotStopParse();
    void slotGetStatus();
};

#endif // WORKER_H
