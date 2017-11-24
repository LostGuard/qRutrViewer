#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>

#include <QSqlDatabase>
#include <QSqlQuery>

#include "rutritem.h"
#include "settings.h"

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModel(QObject *parent, QMap<int, QString> forums);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QString getCategoryText(int id);
    RuTrItem* getItem(int row);
    
public slots:
    void slotSearchFinished(QList<RuTrItem*>* items, int searchTime, int offset);

private:
    QList<RuTrItem*> m_RootItems;
    QMap<int, QString> m_forums;
    int m_offset = 0;
    static QString convertSize(QString size);
};

#endif // TABLEMODEL_H
