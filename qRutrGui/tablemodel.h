#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>

#include <QSqlDatabase>
#include <QSqlQuery>

#include "rutritem.h"

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    RuTrItem* getItem(int row);
public slots:
    void slotSearchFinished(QList<RuTrItem*>* items);

private:
    QList<RuTrItem*> m_RootItems;
};

#endif // TABLEMODEL_H
