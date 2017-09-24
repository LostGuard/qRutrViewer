#include "categorydelegate.h"
#include <QLabel>
#include <QTextDocument>

categoryDelegate::categoryDelegate()
{
    
}

categoryDelegate::~categoryDelegate()
{
    
}

void categoryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if( option.state & QStyle::State_Selected )
            painter->fillRect( option.rect, option.palette.highlight() );
    
    
        painter->save();
    
        QTextDocument document;                         // <---- RichText 
        document.setTextWidth(option.rect.width());
    
        QVariant value = index.data(Qt::DisplayRole);
        if (value.isValid() && !value.isNull())
        {
            document.setHtml("<a href=\"\">" + value.toString() + "</a>");        // <---- make sure model contains html
    
            painter->translate(option.rect.topLeft());
            document.drawContents(painter);        
        }
    
        painter->restore();
}

