#include "itemviewform.h"
#include "ui_itemviewform.h"
//#include <QFile>

ItemViewForm::ItemViewForm(QString content, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemViewForm)
{
    ui->setupUi(this);
    ui->webView->settings()->setUserStyleSheetUrl(QUrl("qrc:/styles.css"));
    ui->webView->setHtml(GetHTML(content));

    m_ReplaceMap.insert("[br]", "<span class=\"post-br\"><br></span>");
    m_ReplaceMap.insert("[b]", "<span class=\"post-b\">");
    m_ReplaceMap.insert("[/b]", "</span>");
    m_ReplaceMap.insert("[i]", "<span class=\"post-i\">");
    m_ReplaceMap.insert("[/i]", "</span>");
    m_ReplaceMap.insert("[u]", "<span class=\"post-u\">");
    m_ReplaceMap.insert("[/u]", "</span>");
    m_ReplaceMap.insert("[s]", "<span class=\"post-s\">");
    m_ReplaceMap.insert("[/s]", "</span>");
    m_ReplaceMap.insert("[hr]", "<hr>");
    m_ReplaceMap.insert("[pre]", "<pre class=\"post-pre\">");
    m_ReplaceMap.insert("[/pre]", "</pre>");
    m_ReplaceMap.insert("[box]", "<div class=\"post-box\">");
    m_ReplaceMap.insert("[/box]", "</div>");
    m_ReplaceMap.insert("[code]", "<div class=\"c-wrap\"><div class=\"c-head\"><b>Код:</b></div><div class=\"c-body\">");
    m_ReplaceMap.insert("[/code]", "</div></div>");

    m_ReplaceMap.insert("[/font]", "</span>");
    m_ReplaceMap.insert("[/color]", "</span>");
    m_ReplaceMap.insert("[/size]", "</span>");
    m_ReplaceMap.insert("[/font]", "</span>");
    m_ReplaceMap.insert("[/align]", "</span>");
    m_ReplaceMap.insert("[/quote]", "</div></div>");
    m_ReplaceMap.insert("[/spoiler]", "<div class=\"sp-fold clickable\">[свернуть]</div></div></div>");
    m_ReplaceMap.insert("[/url]", "</a>");
}

ItemViewForm::~ItemViewForm()
{
    delete ui;
}

QString ItemViewForm::ConvertTag(QString bbcode)
{
    QString res;
    bbcode = bbcode.toLower();

    if (!bbcode.contains("="))
    {
        if (m_ReplaceMap.contains(bbcode))
            return m_ReplaceMap[bbcode];
    }

    res = bbcode;
    bbcode = bbcode.replace("[", "").replace("]", "");

    QStringList list;

    if (bbcode.contains("font", Qt::CaseInsensitive))
    {
        list = bbcode.split("=");
        res = "<span class=\"post-font-" + list[1] + "\">";
    }
    else if (bbcode.contains("color", Qt::CaseInsensitive))
    {
        list = bbcode.split("=");
        res = "<span style=\"color:" + list[1] + "\">";
    }
    else if (bbcode.contains("size", Qt::CaseInsensitive))
    {
        list = bbcode.split("=");
        res = "<span style=\"font-size: " + list[1] + "px; line-height: normal;\">";
    }
    else if (bbcode.contains("align", Qt::CaseInsensitive))
    {
        list = bbcode.split("=");
        res = "<span class=\"post-align\" style=\"text-align: " + list[1] + ";\">";
    }
    else if(bbcode.contains("quote", Qt::CaseInsensitive))
    {
        QString arg = "Цитата: ";
        if (bbcode.contains("="))
        {
            arg = bbcode.split("\"").at(1);
        }
        res = "<div class=\"q-wrap\"><div class=\"q-head\"><span><b>" + arg + "</b> писал(а):</span></div><div class=\"q\">";
    }
    else if(bbcode.contains("spoiler", Qt::CaseInsensitive))
    {
        QString c = "Скрытый текст: ";
        if (bbcode.contains("="))
        {
            c = bbcode.split("\"").at(1);
        }
        res = "<div class=\"sp-wrap clearfix sp-opened\"> <div class=\"sp-head folded unfolded\"><span>" + c + "</span></div><div class=\"sp-body clearfix inited\" style=\"display: block;\">";
    }
    else if(bbcode.contains("url", Qt::CaseInsensitive))
    {
        if (bbcode.contains("="))
        {
            QString c = bbcode.split("=").at(1).split("]").at(0);
            res = "<a href=\"" + c + "\" class=\"postLink\">";
        }
    }

    return res;
}

QString ItemViewForm::GetHTML(QString content)
{
    //QFile file(":/bbcodetest.txt");
    //file.open(QFile::ReadOnly | QFile::Text);
    //QString res = QString(file.readAll());

    QString res = content;
    res = res.replace("\r\n", "<br>");
    res = res.replace("\n", "<br>");

    QRegExp rx("\\[.+\\]");
    rx.setMinimal(true);
    int pos = 0;
    while (pos >= 0)
    {
        pos = rx.indexIn(res, pos);
        if (pos >= 0)
        {
            QString find = res.mid(pos, rx.matchedLength());
            QString newStr = ConvertTag(find);
            res = res.remove(pos, rx.matchedLength());
            res = res.insert(pos, newStr);
            pos += newStr.length();
        }
    }

    rx.setPattern("\\[img\\](.+)\\[/img\\]");
    rx.setMinimal(true);
    rx.setCaseSensitivity(Qt::CaseInsensitive);
    pos = 0;
    while (pos >= 0)
    {
        pos = rx.indexIn(res);
        if (pos >= 0)
        {
            QString s = "<var class=\"postImg\" title=\"" + rx.cap(1) + "\"><img src=\"" + rx.cap(1) + "\" class=\"postImg\" alt=\"pic\"></var>";
            res = res.remove(pos, rx.matchedLength());
            res = res.insert(pos, s);
        }
    }

    rx.setPattern("\\[img=right\\](.+)\\[/img\\]");
    rx.setMinimal(true);
    rx.setCaseSensitivity(Qt::CaseInsensitive);
    pos = 0;
    while (pos >= 0)
    {
        pos = rx.indexIn(res);
        if (pos >= 0)
        {
            QString s = "<var class=\"postImg postImgAligned img-right\" title=\""+ rx.cap(1) +"\"><img src=\""+ rx.cap(1) +"\" class=\"postImg postImgAligned img-right\" alt=\"pic\"></var>";
            res = res.remove(pos, rx.matchedLength());
            res = res.insert(pos, s);
        }
    }

    rx.setPattern("\\[list\\](.+)\\[/list\\]");
    rx.setMinimal(true);
    rx.setCaseSensitivity(Qt::CaseInsensitive);
    pos = 0;
    while (pos >= 0)
    {
        pos = rx.indexIn(res);
        if (pos >= 0)
        {
            QString s = "<ol class=\"post-ul\">";
            QString cap = rx.cap(1);
            QStringList lst = cap.split("[*]");
            if (lst[0].contains(QRegExp("[А-Яа-яЁёA-Za-z]+")))
                s += "<li style=\"list-style: none\">" + lst[0].replace("<br>", "") + "<br></li>";
            lst.removeAt(0);
            foreach (QString si, lst)
            {
                s += "<li>" + si + "</li>";
            }
            s += "</ol>";
            res = res.remove(pos, rx.matchedLength());
            res = res.insert(pos, s);
        }
    }

    QString fin = "<div class=\"message td2\">" ;
    fin += "<div class=\"post_head\"><div>";
    fin += "<div class=\"post_body\" id=\"p-9965519\">";
    fin += res + "</div></div>";
    return fin;
}
