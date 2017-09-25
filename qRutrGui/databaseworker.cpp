#include "databaseworker.h"

DataBaseWorker::DataBaseWorker(DataBase *db)
{
    m_db = db;
}

DataBaseWorker::~DataBaseWorker()
{
    delete m_words;
}

void DataBaseWorker::SetSearchWorker(QStringList* words, int offset, int count, int category_id)
{
    m_recType = SearchType;
    m_words = words;
    m_offset = offset;
    m_count = count;
    m_categoryId = category_id;
}

void DataBaseWorker::SetRequestContentWorker(RuTrItem *item)
{
    m_recType = GetContentType;
    m_item = item;
}

void DataBaseWorker::run()
{
    if (m_recType == SearchType)
    {
        QList<RuTrItem*>* items = new QList<RuTrItem*>();
        m_db->Search(items, *m_words, m_offset, m_count, m_categoryId);
        emit signalSearchFinished(items, 0);
    }
    else if (m_recType == GetContentType)
    {
        QString res = m_db->GetContent(m_item);
        emit signalGetContentFinished(m_item, res);
    }
    return;
}


