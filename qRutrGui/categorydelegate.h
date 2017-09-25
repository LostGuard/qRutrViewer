#ifndef CATEGORYDELEGATE_H
#define CATEGORYDELEGATE_H

#include <QItemDelegate>
#include <QPainter>

class CategoryDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    CategoryDelegate();
    ~CategoryDelegate();
    //void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // CATEGORYDELEGATE_H
