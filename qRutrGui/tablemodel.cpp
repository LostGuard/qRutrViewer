#include "tablemodel.h"
#include <QFont>

TableModel::TableModel(QObject *parent, QMap<int, QString> forums)
{
    m_RootItems = QList<RuTrItem*>();
    m_forums = forums;
}


int TableModel::rowCount(const QModelIndex &parent) const
{
    return m_RootItems.count();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
            return m_forums[m_RootItems[index.row()]->ForumId];
        if (index.column() == 1)
            return m_RootItems[index.row()]->title;
    }

    if (role == Qt::ToolTipRole)
        if (index.column() == 0)
            return index.data(Qt::DisplayRole);
        else
            return QVariant();

    return QVariant();
}

QString TableModel::getCategoryText(int id)
{
    return m_forums[id];
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
