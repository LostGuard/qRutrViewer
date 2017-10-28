#ifndef ITEMVIEWFORM_H
#define ITEMVIEWFORM_H

#include <QWidget>
#include <QWebView>

namespace Ui {
class ItemViewForm;
}

class ItemViewForm : public QWidget
{
    Q_OBJECT

public:
    explicit ItemViewForm(QString content, QString title, QWidget *parent = 0);
    ~ItemViewForm();

private:
    Ui::ItemViewForm *ui;
    QString GetHTML(QString content);
    QString ConvertTag(QString bbcode);
    void replaceRegExp(QRegExp& exp, QString& res, QString& str);

    QMap<QString, QString> m_ReplaceMap;
    QMap<QString, QString> m_IconMap;
};

#endif // ITEMVIEWFORM_H
