#include "tablemodel.h"
#include <QFont>

TableModel::TableModel(QObject *parent)
{
    m_RootItems = QList<RuTrItem*>();
}


int TableModel::rowCount(const QModelIndex &parent) const
{
    return m_RootItems.count();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
            return m_RootItems[index.row()]->title;
    }
    else if (role == Qt::ToolTipRole)
    {
        if (index.column() == 0)
            return m_RootItems[index.row()]->title;
        else
            return QVariant();
    }

    return QVariant();
}

RuTrItem *TableModel::getItem(int row)
{
    return m_RootItems[row];
}

void TableModel::slotSearchFinished(QList<RuTrItem *> *items)
{
    beginResetModel();
    qDeleteAll(m_RootItems);
    m_RootItems.clear();
    m_RootItems.append(*items);
    delete items;// delete list but not the items inside
    endResetModel();
}
