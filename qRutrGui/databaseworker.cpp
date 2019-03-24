#include "databaseworker.h"

DataBaseWorker::DataBaseWorker(DataBase *db)
{
    m_db = db;
}

DataBaseWorker::~DataBaseWorker()
{

}

void DataBaseWorker::SetSearchWorker(QString searchStr, int offset, int count, QList<int> categories, bool fastSearch)
{
    m_recType = SearchType;
    m_searchStr = searchStr;
    m_offset = offset;
    m_count = count;
    m_categories = categories;
    m_fastSearch = fastSearch;
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
        QList<RuTrItem*>* items = new QList<RuTrItem*>;
        if (m_fastSearch)
        {
            m_db->FastSearch(items, m_searchStr, m_offset, m_count, m_categories);
            emit signalSearchFinished(items, 0, m_offset);
        }
        else
        {
            QList<RuTrItem*>* items = new QList<RuTrItem*>();
            QStringList slist;
            if (!m_searchStr.isEmpty())
                slist = m_searchStr.split(" ");
            m_db->Search(items, slist, m_offset, m_count, 0);
            emit signalSearchFinished(items, 0, m_offset);
        }
    }
    else if (m_recType == GetContentType)
    {
        QString res = m_db->GetContent(m_item);
        emit signalGetContentFinished(m_item, res);
    }
    return;
}


