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
    explicit ItemViewForm(QString content, QWidget *parent = 0);
    ~ItemViewForm();

private:
    Ui::ItemViewForm *ui;
    QString GetHTML(QString content);
    QString ConvertTag(QString bbcode);

    QMap<QString, QString> m_ReplaceMap;
    QMap<QString, QString> m_IconMap;
    QMap<QString, QString> m_IconMapFirstParse;
};

#endif // ITEMVIEWFORM_H
