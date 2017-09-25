#include "categorydelegate.h"
#include <QLabel>
#include <QTextDocument>

CategoryDelegate::CategoryDelegate()
{
    
}

CategoryDelegate::~CategoryDelegate()
{
    
}

QSize CategoryDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(option.rect.width(), option.fontMetrics.height());
}

