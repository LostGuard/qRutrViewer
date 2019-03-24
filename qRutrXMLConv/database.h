#ifndef DB_H
#define DB_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QString>
#include <QMap>
#include "rutritem.h"

class DataBase
{
public:
    DataBase();
    DataBase(QString pathToDb);
    ~DataBase();
    bool init(bool fastClearDb);
    void saveRuTrItemCached(RuTrItem* item);
    void sync();
    void saveForums(QMap<int, QString>& map);

    void Search(QList<RuTrItem*>* result, QStringList& keyWords, int offset, int count, int categoryId = -1);
    void FastSearch(QList<RuTrItem*>* result, QString keyWords, int offset, int count, QList<int> categories);

    QString GetContent(RuTrItem* item);
    void resetData();
    QMap<int, QString> getCategories();
    void setWriteMode(bool writeBase, bool writeContent);

    QString errorString;
private:
    QString m_dbpath;
    QSqlQuery* m_QueryBase;
    QSqlQuery* m_QueryContent;
    QSqlQuery* m_QueryTitles;
    QSqlDatabase m_base_db;
    QSqlDatabase m_content_db;
    QList<RuTrItem*> m_Items;
    QString MakeStrFromIntList(QList<int> list);

    RuTrItem* MakeBaseItem(QSqlQuery* query);
    bool m_saveBase = true;
    bool m_saveContent = true;

private:
    QString CON_BASE_DB_NAME = "base_db.sqlite";
    QString CON_CONTENT_DB_NAME = "content_db.sqlite";

    QString CON_BASE_TABLE_NAME = "rutr_base";
    QString CON_CAT_TABLE_NAME = "rutr_cat";
    QString CON_CONTENT_TABLE_NAME = "rutr_content";
    QString CON_TITLES_TABLE_NAME = "rutr_titles";

    QString CON_DATE_TIME_FORMAT = "yyyy.MM.dd hh:mm:ss";
    int CON_CACHE_COUNT = 256;

    void InternalSearch(QList<RuTrItem*>* result, QString whereStr, int offset, int count);
};

#endif // DB_H
