#include "tablemodel.h"
#include <QColor>
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
    return 6;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) 
        {
            if (section == 0)
                return "Id";
            else if (section == 1)
                return "Категория";
            else if (section == 2)
                return "Заголовок";
            else if (section == 3)
                return "Размер";
            else if (section == 4)
                return "Время";
            else if (section == 5)
                return "Хеш";
            else
                return QVariant();
        }
        if (orientation == Qt::Vertical) 
        {
            return m_offset + section + 1;
        }
    }
    return QVariant();
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
            return QString::number(m_RootItems[index.row()]->id);
        else if (index.column() == 1)
            return m_forums[m_RootItems[index.row()]->ForumId];
        else if (index.column() == 2)
            return m_RootItems[index.row()]->title;
        else if (index.column() == 3)
            return convertSize(m_RootItems[index.row()]->size);
        else if (index.column() == 4)
            return m_RootItems[index.row()]->registered.toString("hh:mm:ss dd.MM.yyyy ");
        else if (index.column() == 5)
            return m_RootItems[index.row()]->TorrentHash;
        else
            return QVariant();
    }

    if (role == Qt::BackgroundRole)
        if (index.row() % 2 == 0)
            return  QColor(255,255,235);
    
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

void TableModel::slotSearchFinished(QList<RuTrItem *> *items, int searchTime, int offset)
{
    beginResetModel();
    qDeleteAll(m_RootItems);
    m_RootItems.clear();
    m_RootItems.append(*items);
    delete items;// delete list but not the items inside
    m_offset = offset;
    endResetModel();
}

QString TableModel::convertSize(QString size)
{
    size = size.trimmed();
    QString res = "";
    if (size.length() > 12)
        res = size.left(size.length() - 11) + "Тб";
    else if (size.length() > 9)
        res = size.left(size.length() - 8) + "Гб";
    else if (size.length() > 6)
        res = size.left(size.length() - 5) + "Мб";
    else if (size.length() > 3)
        res = size.left(size.length() - 2) + "Кб";
    
    if (!res.isEmpty())
        res = res.insert(res.length() - 3, '.');
    else
        res = size;
    
    return res;
}
