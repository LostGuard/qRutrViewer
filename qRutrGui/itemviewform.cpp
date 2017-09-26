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
    QString res = content;
    res = res.replace("\r\n", "<br>");
    res = res.replace("\n", "<br>");

    QRegExp rx("\\[.+\\]"); // соответствие Eric или Eirik
    rx.setMinimal(true);
    int pos = 0;    // где мы находимся в строке
    while (pos >= 0)
    {
        pos = rx.indexIn(res, pos);
        if (pos >= 0)
        {
            QString find = res.mid(pos, rx.matchedLength());
            qDebug() << find;
            QString newStr = ConvertTag(find);
            qDebug() << ConvertTag(find);
            res = res.remove(pos, rx.matchedLength());
            res = res.insert(pos, newStr);
            pos += newStr.length();      // двигаемся далее по строке
        }
    }

    res = res.replace("[list][*]", "<ol><li>");
    res = res.replace("[list]", "<ol>");
    res = res.replace("[*]", "</li><li>");
    res = res.replace("[/list]", "</ol>");


    return res;
}

QString ItemViewForm::ConvertTag(QString bbcode)
{
    QMap<QString, QString> map;
    map.insert("[br]", "<br>");
    map.insert("[b]", "<span style=\"font-weight: bold\">");
    map.insert("[/b]", "</span>");
    map.insert("[i]", "<span style=\"font-style: italic\">");
    map.insert("[/i]", "</span>");
    map.insert("[u]", "<span style=\"text-decoration: underline\">");
    map.insert("[/u]", "</span>");
    map.insert("[s]", "<span style=\"text-decoration: line-through\">");
    map.insert("[/s]", "</span>");
    map.insert("[img=right]", "<img style=\"float:right\" src=\"");
    map.insert("[img]", "<img src=\"");
    map.insert("[/img]", "\">");
    map.insert("[hr]", "<hr>");
    map.insert("[pre]", "<pre>");
    map.insert("[/pre]", "</pre>");
    map.insert("[box]", "<span style=\"display:inline-block;border:1px solid #a7b4ba;border-radius:3px;line-height: 150%;margin: 1px 5px;padding: 0 8px 1px;\">");
    map.insert("[/box]", "</span>");
    map.insert("[quote]", "<div style=\"width: 98%;clear: both;\"><div style=\"text-align:left !important;min-width: 150px;min-height: 14px;\"><span>Цитата: </span></div><div style=\"background: #fafafa;color:#2e2e2e;max-height: 350px;overflow: auto;\">");
    map.insert("[/quote]", "</div></div>");
    map.insert("[code]", "<div style=\"width: 98%;clear: both;\"><div style=\"text-align:left !important;min-width: 150px;min-height: 14px;\"><span>Код: </span></div><div style=\"background: #fafafa;color:#2e2e2e;max-height: 350px;overflow: auto;\">");
    map.insert("[/code]", "</div></div>");
    //Не соответствует сайту, скорее всего нужно через javascript
    map.insert("[spoiler]", "<div style=\"width: 98%;clear: both;\"><div style=\"text-align:left !important;min-width: 150px;min-height: 14px;\"><span> скрытый текст </span></div><div style=\"background: #fafafa;color:#2e2e2e;max-height: 350px;overflow: auto;\">");
    map.insert("[/spoiler]", "</div></div>");

    map.insert("[/url]", "</a>");
    map.insert("[/font]", "</span>");
    map.insert("[/color]", "</span>");
    map.insert("[/size]", "</span>");
    map.insert("[/font]", "</span>");

    QString res;
    bbcode = bbcode.toLower();

    if (bbcode.contains("=") && bbcode != "[img=right]")
    {
        res = bbcode;
        bbcode = bbcode.replace("[", "").replace("]", "");

        QStringList list = bbcode.split("=");
        //list[0].trim
        if (QString::compare(list[0], "URL", Qt::CaseInsensitive) == 0)
        {
            res = "<a href=\"" + list[1] + "\">";
        }
        else if (QString::compare(list[0], "font", Qt::CaseInsensitive) == 0)
        {
            res = "<span style=\"font-family:" + list[1] + "\">";
        }
        else if (QString::compare(list[0], "color", Qt::CaseInsensitive) == 0)
        {
            res = "<span style=\"color:" + list[1] + "\">";
        }
        else if (QString::compare(list[0], "size", Qt::CaseInsensitive) == 0)
        {
            res = "<span style=\"font-size:" + list[1] + "px\">";
        }
        else if (QString::compare(list[0], "align", Qt::CaseInsensitive) == 0)
        {
            res = "<span style=\"text-align:" + list[1] + "\">";
        }
        else if (QString::compare(list[0], "quote", Qt::CaseInsensitive) == 0)
        {
            res = "<div style=\"width: 98%;clear: both;\"><div style=\"text-align:left !important;min-width: 150px;min-height: 14px;\"><span>" + list[1] + "</span></div><div style=\"background: #fafafa;color:#2e2e2e;max-height: 350px;overflow: auto;\">";
        }
        else if (QString::compare(list[0], "code", Qt::CaseInsensitive) == 0)
        {
            res = "<div style=\"width: 98%;clear: both;\"><div style=\"text-align:left !important;min-width: 150px;min-height: 14px;\"><span>" + list[1] + "</span></div><div style=\"background: #fafafa;color:#2e2e2e;max-height: 350px;overflow: auto;\">";
        }
        else if (QString::compare(list[0], "spoiler", Qt::CaseInsensitive) == 0)
        {
            res = "<div style=\"width: 98%;clear: both;\"><div style=\"text-align:left !important;min-width: 150px;min-height: 14px;\"><span>" + list[1] + "</span></div><div style=\"background: #fafafa;color:#2e2e2e;max-height: 350px;overflow: auto;\">";
        }
    }
    else
    {
        if (map.contains(bbcode))
            res = map[bbcode];
        else
            res = bbcode;
    }
    return res;
}
/*
[font=serif][/font]
[color=darkred][/color]
[size=11][/size]
[align=right][/align]
[img=right][/img]
[br]
[pre][/pre]
[box][/box]
[b][/b]
[i][/i]
[u][/u]
[s][/s]
[quote][/quote]
[img][/img]
[url=][/url]

[list][/list]
[*]

[hr]
[code][/code]
[spoiler][/spoiler]
*/
