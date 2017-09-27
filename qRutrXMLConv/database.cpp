#include "database.h"
#include <QVariantList>
#include <QDir>
#include <QFile>
#include <QSqlError>


DataBase::DataBase()
{
    m_dbpath = "." + QString(QDir::separator());
}

DataBase::DataBase(QString pathToDb)
{
    if (pathToDb.isEmpty())
        m_dbpath = "." + QString(QDir::separator());
    else
        if (!pathToDb.endsWith(QDir::separator()))
            m_dbpath = pathToDb + QDir::separator();
        else
            m_dbpath = pathToDb;
}

DataBase::~DataBase()
{
    m_base_db.close();
    m_content_db.close();
}

bool DataBase::init(bool fastClearDb)
{
    if(fastClearDb)
    {
        if (m_saveBase)
        {
            if (QFile::exists(m_dbpath + CON_BASE_DB_NAME))
            {
                if (!QFile::remove(m_dbpath + CON_BASE_DB_NAME))
                {
                    errorString += "Не удалось удалить файлы базы " + CON_BASE_DB_NAME;
                    return false;
                }
            }
        }
        if (m_saveContent)
        {
            if (QFile::exists(m_dbpath + CON_CONTENT_DB_NAME))
            {
                if (!QFile::remove(m_dbpath + CON_CONTENT_DB_NAME))
                {
                    errorString += "Не удалось удалить файлы базы" + CON_CONTENT_DB_NAME;
                    return false;
                }
            }
        }
    }

    m_base_db = QSqlDatabase::addDatabase("QSQLITE", "base");
    m_base_db.setDatabaseName(m_dbpath + CON_BASE_DB_NAME);

    m_content_db = QSqlDatabase::addDatabase("QSQLITE", "content");
    m_content_db.setDatabaseName(m_dbpath + CON_CONTENT_DB_NAME);

    if (!m_base_db.open()) {
        errorString += "open base db error" + m_base_db.lastError().text();
        return false;
    }

    if (!m_content_db.open()) {
        errorString += "open content db error" + m_base_db.lastError().text();
        return false;
    }

    m_QueryBase = new QSqlQuery(m_base_db);
    m_QueryBase->exec("CREATE TABLE IF NOT EXISTS " + CON_BASE_TABLE_NAME + " (id INTEGER PRIMARY KEY, size TEXT, hash TEXT, forum_id INTEGER, registered TEXT)");
    m_QueryBase->exec("CREATE TABLE IF NOT EXISTS " + CON_CAT_TABLE_NAME + " (id INTEGER PRIMARY KEY, description TEXT)");
    m_QueryBase->exec("CREATE VIRTUAL TABLE IF NOT EXISTS " + CON_TITLES_TABLE_NAME + " USING fts4(title TEXT)");

    m_QueryContent = new QSqlQuery(m_content_db);
    m_QueryContent->exec("CREATE TABLE IF NOT EXISTS " + CON_CONTENT_TABLE_NAME + "  (id INTEGER PRIMARY KEY, content BLOB)");

    return true;
}

void DataBase::saveRuTrItemCached(RuTrItem *item)
{
    if (m_Items.count() >= CON_CACHE_COUNT)
        sync();
    m_Items.append(item);
}

void DataBase::sync()
{
    QVariantList ids;
    QVariantList titles;
    QVariantList sizes;
    QVariantList hashes;
    QVariantList forum_ids;
    QVariantList contents;
    QVariantList registered;

    foreach (RuTrItem* it, m_Items)
    {
         ids.append(it->id);
         titles.append(it->title);
         sizes.append(it->size);
         hashes.append(it->TorrentHash);
         forum_ids.append(it->ForumId);
         contents.append(qCompress(it->content.toUtf8(), 9));
         registered.append(it->registered.toString(CON_DATE_TIME_FORMAT));
    }

    if (m_saveBase)
    {
        m_QueryBase->clear();
        m_QueryBase->exec("BEGIN TRANSACTION");
        m_QueryBase->prepare("INSERT INTO " + CON_BASE_TABLE_NAME + " (id, size, hash, forum_id, registered) VALUES (?, ?, ?, ?, ?)");
        m_QueryBase->addBindValue(ids);
        m_QueryBase->addBindValue(sizes);
        m_QueryBase->addBindValue(hashes);
        m_QueryBase->addBindValue(forum_ids);
        m_QueryBase->addBindValue(registered);
        m_QueryBase->execBatch();
        m_QueryBase->exec("END TRANSACTION");

        m_QueryBase->clear();
        m_QueryBase->exec("BEGIN TRANSACTION");
        m_QueryBase->prepare("INSERT INTO " + CON_TITLES_TABLE_NAME + " (docid, title) VALUES (?, ?)");
        m_QueryBase->addBindValue(ids);
        m_QueryBase->addBindValue(titles);
        m_QueryBase->execBatch();
        m_QueryBase->exec("END TRANSACTION");

    }
    //=================
    if (m_saveContent)
    {
        m_QueryContent->clear();
        m_QueryContent->exec("BEGIN TRANSACTION");
        m_QueryContent->prepare("INSERT INTO " + CON_CONTENT_TABLE_NAME + " (id, content) VALUES (?, ?)");

        m_QueryContent->addBindValue(ids);
        m_QueryContent->addBindValue(contents);

        m_QueryContent->execBatch();
        m_QueryContent->exec("END TRANSACTION");
    }
    //=================
    qDeleteAll(m_Items);
    m_Items.clear();
}

void DataBase::saveForums(QMap<int, QString>& map)
{
    if (m_saveBase)
    {
        QVariantList ids;
        QVariantList descrs;

        foreach (int k, map.keys())
        {
            ids << k;
            descrs << map[k];
        }

        m_QueryBase->clear();
        m_QueryBase->exec("BEGIN TRANSACTION");
        m_QueryBase->prepare("INSERT INTO " + CON_CAT_TABLE_NAME + " (id, description) VALUES (?, ?)");
        m_QueryBase->addBindValue(ids);
        m_QueryBase->addBindValue(descrs);

        m_QueryBase->execBatch();
        m_QueryBase->exec("END TRANSACTION");
    }
}

void DataBase::Search(QList<RuTrItem*>* result, QStringList& keyWords, int offset, int count, int categoryId)
{
    QString whereStr;

    if (keyWords.count() != 0 || categoryId != -1)
    {
        whereStr += " WHERE ";
        foreach (QString k, keyWords)
            whereStr += "title like '%" + k.trimmed() + "%' and ";

        if (categoryId != -1)
            whereStr += "forum_id = " + QString::number(categoryId) + " and";

        whereStr = whereStr.left(whereStr.count() - 4); // remove last 'and'
    }

    InternalSearch(result, whereStr, offset, count);
}

void DataBase::FastSearch(QList<RuTrItem*>* result, QString keyWords, int offset, int count, int categoryId)
{//SELECT * FROM rutr_titles  join rutr_base ON rutr_titles.docid = rutr_base.id where rutr_titles.title match "linux windows" and forum_id = 1424 order by id limit 3
    QString whereStr;

    if (!keyWords.isEmpty() || categoryId != -1)
    {
        whereStr += "WHERE ";
        if (!keyWords.isEmpty())
            whereStr += CON_TITLES_TABLE_NAME + ".title " + "match \"" + keyWords + "\"" + " and";
        if (categoryId != -1)
            whereStr += " forum_id=" + QString::number(categoryId) + " and";

        whereStr = whereStr.left(whereStr.count() - 4); // remove last 'and'
    }

    InternalSearch(result, whereStr, offset, count);
}

void DataBase::InternalSearch(QList<RuTrItem*>* result, QString whereStr, int offset, int count)
{//SELECT * FROM rutr_titles  join rutr_base ON rutr_titles.docid = rutr_base.id where rutr_titles.title match "linux windows" and forum_id = 1424 order by id limit 3
    m_QueryBase->clear();
    QString cmd = "SELECT * FROM " + CON_TITLES_TABLE_NAME + " join " + CON_BASE_TABLE_NAME + " ON " + CON_TITLES_TABLE_NAME + ".docid = " + CON_BASE_TABLE_NAME + ".id ";

    cmd += whereStr;

    cmd += " ORDER BY id DESC";
    cmd += " LIMIT " + QString::number(offset) + "," + QString::number(count);

    m_QueryBase->prepare(cmd);
    m_QueryBase->exec();
    while (m_QueryBase->next())
    {
        RuTrItem* item = MakeBaseItem(m_QueryBase);
        result->append(item);
    }
}

//title, id, size, hash, forum_id, registered
RuTrItem *DataBase::MakeBaseItem(QSqlQuery *query)
{
    RuTrItem *item = new RuTrItem();
    item->id = query->value(1).toInt();
    item->title = query->value(0).toString();
    item->size = query->value(2).toString();
    item->TorrentHash = query->value(3).toString();
    item->ForumId = query->value(4).toInt();
    item->registered = QDateTime::fromString(query->value(5).toString(), CON_DATE_TIME_FORMAT);
    return item;
}

QString DataBase::GetContent(RuTrItem *item)
{
    QString result = "";
    m_QueryContent->clear();
    QString cmd = "SELECT content FROM " + CON_CONTENT_TABLE_NAME + " WHERE id = " + QString::number(item->id);
    m_QueryContent->prepare(cmd);
    m_QueryContent->exec();
    if (m_QueryContent->next())
        result = qUncompress(m_QueryContent->value(0).toByteArray());
    return result;
}

void DataBase::resetData()
{
    m_QueryContent->exec("PRAGMA synchronous = OFF");
    m_QueryContent->exec("PRAGMA journal_mode = MEMORY");
    m_QueryBase->exec("PRAGMA synchronous = OFF");
    m_QueryBase->exec("PRAGMA journal_mode = MEMORY");

    if (m_saveContent)
        m_QueryContent->exec("DELETE FROM " + CON_CONTENT_TABLE_NAME);

    if (m_saveBase)
    {
        m_QueryBase->exec("DELETE FROM " + CON_BASE_TABLE_NAME);
        m_QueryBase->exec("DELETE FROM " + CON_CAT_TABLE_NAME);
        m_QueryBase->exec("DELETE FROM " + CON_TITLES_TABLE_NAME);
    }
}

QMap<int, QString> DataBase::getCategories()
{
    QMap<int, QString> res;
    m_QueryBase->clear();
    QString cmd = "SELECT * FROM " + CON_CAT_TABLE_NAME;
    m_QueryBase->prepare(cmd);
    m_QueryBase->exec();
    while (m_QueryBase->next())
        res.insert(m_QueryBase->value(0).toInt(), m_QueryBase->value(1).toString());
    return res;
}

void DataBase::setWriteMode(bool writeBase, bool writeContent)
{
    m_saveBase = writeBase;
    m_saveContent = writeContent;
}

