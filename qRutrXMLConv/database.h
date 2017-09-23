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

    void Search(QList<RuTrItem*>* result, QStringList& keyWords, int start, int end, int categoryId = -1);
    QString GetContent(RuTrItem* item);
    void resetData();
    QMap<int, QString> getCategories();
    void setWriteMode(bool writeBase, bool writeContent, bool writeForums);

    QString errorString;
private:
    QString m_dbpath;
    QSqlQuery* m_QueryBase;
    QSqlQuery* m_QueryContent;
    QSqlDatabase m_base_db;
    QSqlDatabase m_content_db;
    QList<RuTrItem*> m_Items;

    RuTrItem* MakeBaseItem(QSqlQuery* query);
    bool m_saveBase = true;
    bool m_saveContent = true;
    bool m_saveForums = true;

private:
    QString CON_BASE_DB_NAME = "base_db.sqlite";
    QString CON_CONTENT_DB_NAME = "content_db.sqlite";
    QString CON_BASE_TABLE_NAME = "rutr_base";
    QString CON_CAT_TABLE_NAME = "rutr_cat";
    QString CON_CONTENT_TABLE_NAME = "rutr_content";
    QString CON_DATE_TIME_FORMAT = "yyyy.MM.dd hh:mm:ss";
    int CON_CACHE_COUNT = 256;
};

#endif // DB_H
