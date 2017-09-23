#include "itemviewform.h"
#include "ui_itemviewform.h"

ItemViewForm::ItemViewForm(QString content, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemViewForm)
{
    ui->setupUi(this);
    ui->webView->setHtml(GetHTML(content));
}

ItemViewForm::~ItemViewForm()
{
    delete ui;
}

QString ItemViewForm::GetHTML(QString content)
{
    /*
    [font=serif][/font]
    [color=darkred][/color]
    [size=11][/size]
    [align=right][/align]
    -- [img=right][/img]
    -- [br]
    [pre][/pre]
    [box][/box]
    -- [b][/b]
    -- [i][/i]
    -- [u][/u]
    -- [s][/s]
    [quote][/quote]
    -- [img][/img]
    [url=][/url]

    -- [list][/list]
    -- [*]

    -- [hr]
    [code][/code]
    [spoiler][/spoiler]
    */

    QString res = content;
    res = res.replace("[br]", "<br>");
    res = res.replace("\r\n", "<br>");
    res = res.replace("\n", "<br>");

    res = res.replace("[b]", "<span style=\"font-weight: bold\">");
    res = res.replace("[/b]", "</span>");

    res = res.replace("[i]", "<span style=\"font-style: italic\">");
    res = res.replace("[/i]", "</span>");

    res = res.replace("[u]", "<span style=\"text-decoration: underline\">");
    res = res.replace("[/u]", "</span>");

    res = res.replace("[s]", "<span style=\"text-decoration: line-through\">");
    res = res.replace("[/s]", "</span>");

    res = res.replace("[img=right]", "<img style=\"float:right\" src=\"");
    res = res.replace("[img]", "<img src=\"");
    res = res.replace("[/img]", "\">");

    res = res.replace("[list][*]", "<ol><li>");
    res = res.replace("[list]", "<ol>");
    res = res.replace("[*]", "</li><li>");
    res = res.replace("[/list]", "</ol>");

    res = res.replace("[hr]", "<hr>");


    return res;
}
