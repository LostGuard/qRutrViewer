#ifndef ITEMVIEWFORM_H
#define ITEMVIEWFORM_H

#include <QWidget>
#include <QWebView>
#include "rutritem.h"

namespace Ui {
class ItemViewForm;
}

class ItemViewForm : public QWidget
{
    Q_OBJECT

public:
    explicit ItemViewForm(RuTrItem *item, QString content, QWidget *parent = 0);
    ~ItemViewForm();

private slots:
    void on_saveHTMLButton_clicked();
    
    void on_saveBBCodeButton_clicked();
    
    void on_copySiteURLButton_clicked();
    
private:
    Ui::ItemViewForm *ui;
    QString GetHTML(QString content);
    QString ConvertTag(QString bbcode);
    void replaceRegExp(QRegExp& exp, QString& res, QString& str);

    QMap<QString, QString> m_ReplaceMap;
    QMap<QString, QString> m_IconMap;
    
    RuTrItem *m_item;
    QString m_content;
};

#endif // ITEMVIEWFORM_H
