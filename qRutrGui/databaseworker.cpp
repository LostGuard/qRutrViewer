#include "databaseworker.h"

DataBaseWorker::DataBaseWorker(DataBase *db, QStringList *words, int offset, int count, int categoryId)
{
    m_db = db;
    m_words = words;
    m_offset = offset;
    m_count = count;
    m_categoryId = categoryId;
}

DataBaseWorker::~DataBaseWorker()
{
    delete m_words;
}

void DataBaseWorker::run()
{
    QList<RuTrItem*>* items = new QList<RuTrItem*>();
    m_db->Search(items, *m_words, m_offset, m_count, m_categoryId);
    emit signalSearchFinished(items, 0);
    return;
}


