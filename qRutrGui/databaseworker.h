#ifndef DATABASEWORKER_H
#define DATABASEWORKER_H
#include <QThread>
#include <QStringList>
#include "rutritem.h"
#include "../qRutrXMLConv/database.h"

class DataBaseWorker : public QThread
{
    Q_OBJECT
public:
    DataBaseWorker(DataBase *db);
    ~DataBaseWorker();
    void SetSearchWorker(QString searchStr, int offset, int count, int category_id, bool fastSearch);
    void SetRequestContentWorker(RuTrItem *item);

private:
    DataBase* m_db;
    QString m_searchStr;
    int m_offset;
    int m_count;
    int m_categoryId;
    RuTrItem* m_item;
    bool m_fastSearch;

    enum RequestType
    {
        SearchType,
        GetContentType
    };
    RequestType m_recType;

protected:
    void run();

signals:
    void signalError(QString str);
    void signalSearchFinished(QList<RuTrItem*>* items, int searchTimeMs);
    void signalGetContentFinished(RuTrItem* item, QString content);
};

#endif // DATABASEWORKER_H
