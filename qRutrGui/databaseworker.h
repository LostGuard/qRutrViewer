#ifndef DATABASEWORKER_H
#define DATABASEWORKER_H
#include <QThread>
#include "rutritem.h"
#include "../qRutrXMLConv/database.h"

class DataBaseWorker : public QThread
{
    Q_OBJECT
public:
    DataBaseWorker(DataBase *db, QStringList *words, int offset, int count, int categoryId);
    ~DataBaseWorker();

private:
    DataBase* m_db;
    QStringList* m_words;
    int m_offset;
    int m_count;
    int m_categoryId;

protected:
    void run();

signals:
    void signalError(QString str);
    void signalSearchFinished(QList<RuTrItem*>* items, int searchTimeMs);
};

#endif // DATABASEWORKER_H
