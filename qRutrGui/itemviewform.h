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
};

#endif // ITEMVIEWFORM_H
