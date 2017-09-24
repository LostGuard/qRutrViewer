#ifndef CATEGORYDELEGATE_H
#define CATEGORYDELEGATE_H

#include <QItemDelegate>
#include <QPainter>

class categoryDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    categoryDelegate();
    ~categoryDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // CATEGORYDELEGATE_H
