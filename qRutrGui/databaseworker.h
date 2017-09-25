#ifndef DATABASEWORKER_H
#define DATABASEWORKER_H
#include <QThread>
#include "rutritem.h"
#include "../qRutrXMLConv/database.h"

class DataBaseWorker : public QThread
{
    Q_OBJECT
public:
    DataBaseWorker(DataBase *db);
    ~DataBaseWorker();
    void SetSearchWorker(QStringList* words, int offset, int count, int category_id);
    void SetRequestContentWorker(RuTrItem *item);

private:
    DataBase* m_db;
    QStringList* m_words;
    int m_offset;
    int m_count;
    int m_categoryId;
    RuTrItem* m_item;

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
