#include "database.h"
#include <QVariantList>
#include <QDir>
#include <QFile>



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
        if (m_saveBase && m_saveForums)
        {
            if (QFile::exists(m_dbpath + CON_BASE_DB_NAME))
            {
                if (!QFile::remove(m_dbpath + CON_BASE_DB_NAME))
                {
                    errorString += "Не удалось удалить файлы базы";
                    return false;
                }
            }
        }
        if (QFile::exists(m_dbpath + CON_CONTENT_DB_NAME))
        {
            if (!QFile::remove(m_dbpath + CON_CONTENT_DB_NAME))
            {
                errorString += "Не удалось удалить файлы базы";
                return false;
            }
        }
    }

    m_base_db = QSqlDatabase::addDatabase("QSQLITE", "base");
    m_base_db.setDatabaseName(m_dbpath + CON_BASE_DB_NAME);

    m_content_db = QSqlDatabase::addDatabase("QSQLITE", "content");
    m_content_db.setDatabaseName(m_dbpath + CON_CONTENT_DB_NAME);

    if (!m_base_db.open()) {
        errorString += "open base db error";
        return false;
    }

    if (!m_content_db.open()) {
        errorString += "open content db error";
        return false;
    }

    m_QueryBase = new QSqlQuery(m_base_db);
    m_QueryBase->exec("CREATE TABLE IF NOT EXISTS " + CON_BASE_TABLE_NAME + " (id INTEGER PRIMARY KEY, title TEXT, size TEXT, hash TEXT, forum_id INTEGER, registered TEXT)");
    m_QueryBase->exec("CREATE TABLE IF NOT EXISTS " + CON_CAT_TABLE_NAME + " (id INTEGER PRIMARY KEY, description TEXT)");

    m_QueryContent = new QSqlQuery(m_content_db);
    m_QueryContent->exec("CREATE TABLE IF NOT EXISTS " + CON_CONTENT_TABLE_NAME + "  (id INTEGER PRIMARY KEY, content TEXT)");


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
         contents.append(it->content);
         registered.append(it->registered.toString(CON_DATE_TIME_FORMAT));
    }

    if (m_saveBase)
    {
        m_QueryBase->clear();
        m_QueryBase->exec("BEGIN TRANSACTION");
        m_QueryBase->prepare("INSERT INTO " + CON_BASE_TABLE_NAME + " (id, title, size, hash, forum_id, registered) VALUES (?, ?, ?, ?, ?, ?)");
        m_QueryBase->addBindValue(ids);
        m_QueryBase->addBindValue(titles);
        m_QueryBase->addBindValue(sizes);
        m_QueryBase->addBindValue(hashes);
        m_QueryBase->addBindValue(forum_ids);
        m_QueryBase->addBindValue(registered);


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

void DataBase::Search(QList<RuTrItem*>* result, QStringList& keyWords, int start, int end, int categoryId)
{
    m_QueryBase->clear();
    QString cmd = "SELECT * FROM " + CON_BASE_TABLE_NAME;
    QString str = " WHERE ";
    foreach (QString k, keyWords)
    {
        str += "title like '%" + k.trimmed() + "%' and ";
    }

    if (categoryId != -1)
        str += "forum_id = " + QString::number(categoryId);
    else
        str = str.left(str.count() - 4); // remove last 'and'

    str += " ORDER BY id ";
    str += "LIMIT " + QString::number(start) + "," + QString::number(end);
    cmd += str;

    m_QueryBase->prepare(cmd);
    m_QueryBase->exec();
    while (m_QueryBase->next())
    {
        RuTrItem* item = MakeBaseItem(m_QueryBase);
        result->append(item);
    }
}

QString DataBase::GetContent(RuTrItem *item)
{
    QString result = "";
    m_QueryContent->clear();
    QString cmd = "SELECT content FROM " + CON_CONTENT_TABLE_NAME + " WHERE id = " + QString::number(item->id);
    m_QueryContent->prepare(cmd);
    m_QueryContent->exec();
    if (m_QueryContent->next())
        result = m_QueryContent->value(0).toString();
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
        m_QueryBase->exec("DELETE FROM " + CON_BASE_TABLE_NAME);
    if (m_saveForums)
        m_QueryBase->exec("DELETE FROM " + CON_CAT_TABLE_NAME);
}

QMap<int, QString> DataBase::getCategories()
{
    QMap<int, QString> res;
    res.insert(2, "Кино, Видео и ТВ");
    return res;
}

void DataBase::setWriteMode(bool writeBase, bool writeContent, bool writeForums)
{
    m_saveBase = writeBase;
    m_saveContent = writeContent;
    m_saveForums = writeForums;
}
//id, title, size, hash, forum_id, registered
RuTrItem *DataBase::MakeBaseItem(QSqlQuery *query)
{
    RuTrItem *item = new RuTrItem();
    item->id = query->value(0).toInt();
    item->title = query->value(1).toString();
    item->size = query->value(2).toString();
    item->TorrentHash = query->value(3).toString();
    item->ForumId = query->value(4).toInt();
    item->registered = QDateTime::fromString(query->value(5).toString(), CON_DATE_TIME_FORMAT);
    return item;
}

