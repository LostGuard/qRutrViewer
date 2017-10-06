#include "itemviewform.h"
#include "ui_itemviewform.h"
//#include <QFile>
#include <QWebFrame>

ItemViewForm::ItemViewForm(QString content, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemViewForm)
{
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
    
    m_IconMap = {
        {":D", "icon_biggrin"}, {":)", "icon_smile"}, {":(", "icon_sad"}, {":o", "icon_surprised"},{":shock:", "icon_eek"},
        {":?", "icon_confused"}, {":cool:", "icon_cool"}, {":lol:", "icon_lol"}, {":x", "icon_mad"}, {":wow:", "icon_razz"},
        {":blush:", "icon_redface"}, {":cry:", "icon_cry"}, {":evil:", "icon_evil"}, {":twisted:", "icon_twisted"},
        {":roll:", "icon_rolleyes"}, {":wink:", "icon_wink"}, {":!:", "icon_exclaim"}, {":?:", "icon_question"},
        {":idea:", "icon_idea"}, {":arrow:", "icon_arrow"}, {":arrow2:", "icon_arrow2"}, {":angry:", "icon_angry"},
        {":P", "be-e"}, {":mrgreen:", "icon_mrgreen"}, {":boxed:", "icon_boxed"}, {":furious:", "icon_furious"},
        {":greedy:", "icon_greedy"}, {":in_love:", "icon_in_love"}, {":rant:", "icon_rant"}, {":sick:", "icon_sick"},
        {":wall:", "icon_wall"}, {":weep:", "icon_weep"}, {":yawn:", "icon_yawn"}, {":up:", "ges_up"}, {":down:", "ges_down"},
        {":yes:", "ges_yes"}, {":no:", "ges_no"}, {":help:", "ges_help"}, {":bow:", "ges_bow"}, {":clap:", "ges_clap"},
        {":clap2:", "ges_clap2"}, {":hmm:", "ges_hmm"}, {":slap:", "ges_slap"}, {":biker:", "ppl_biker"},
        {":chef:", "ppl_chef"}, {":cylon:", "ppl_cylon"}, {":hannibal:", "ppl_hannibal"}, {":santa:", "santa"},
        {":indian:", "ppl_indian"}, {":king:", "ppl_king"}, {":mario:", "ppl_mario"}, {":ninja:", "ppl_ninja"},
        {":ninjajig:", "ppl_ninjajig"}, {":shuriken:", "ppl_shuriken"},
        {":snegurochka:", "snegurochka"}, {":pimp:", "ppl_pimp"}, {":pirat:", "ppl_pirat"}, {":pirate:", "ppl_pirate"},
        {":police:", "ppl_police"}, {":pop:", "ppl_pop"}, {":priest:", "ppl_priest"}, {":punk:", "ppl_punk"},
        {":rambo:", "ppl_rambo"}, {":rock:", "ppl_rock"}, {":newyear:", "new_year"},{":smurf:", "ppl_smurf"},
        {":spidey:", "ppl_spidey"}, {":wolverine:", "ppl_wolverine"}, {":zorro:", "ppl_zorro"}, {":artist:", "ext_artist"},
        {":argue:", "ext_argue"}, {":baby:", "ext_baby"}, {":beer:", "ext_beer"}, {":beer2:", "ext_beer2"},
        {":book:", "ext_book"}, {":bounce:", "ext_bounce"}, {":box:", "ext_box"}, {":cigar:", "ext_cigar"},
        {":clown:", "ext_clown"}, {":cry_baby:", "ext_crybaby"}, {":crutch:", "ext_crutch"}, {":doc:", "ext_doc"},
        {":drunk:", "ext_drunk"}, {":flex:", "ext_flex"}, {":jump2:", "ext_jump2"}, {":lamo:", "ext_lame"},
        {":komp_cr:", "ext_komp_cr"}, {":hooray:", "ext_hooray"}, {":hump:", "ext_hump"}, {":icecream:", "ext_icecream"},
        {":kiss:", "ext_kiss"}, {":lovers:", "ext_lovers"}, {":mobile:", "ext_mobile"}, {":music:", "ext_music"},
        {":secret:", "ext_secret"},{":shutup:", "ext_shutup"}, {":sleep:", "ext_sleep"}, {":spider:", "ext_spider"},
        {":tease:", "ext_tease"}, {":tomato:", "ext_tomato"}, {":wheelcha:", "ext_wheelcha"}, {":2guns:", "gun_2guns"},
        {":axe:", "gun_axe"}, {":bash:", "gun_bash"}, {":chair:", "gun_chair"}, {":gun:", "gun_gun"},
        {":alien:", "non_alien"}, {":bananadance:", "non_banana1"}, {":bananadance2", "non_banana2"}, {":cat:", "non_cat"},
        {":clover:", "non_clover"}, {":homestar:", "non_homestar"}, {":love:", "non_love"}, {":nuke:", "non_nuke"},
        {":shaun:", "non_shaun"}, {":angel:", "big_angel"}, {":band:", "big_band"}, {":hang:", "big_hang"}, {":hbd:", "big_hbd"},
        {":ban:", "tr_ban"}, {":hi:", "tr_hi"},  {":respect:", "tr_respect"}, {":rip:", "tr_rip"},
        {":RTFM:", "tr_rtfm"}, {":russ:", "tr_russ"}, {":t_oops:", "tr_oops"}, {":sorry:", "tr_sorry"}, {":spam:", "tr_spam"},
        {":thankyou:", "tr_thankyou"}, {":biggrin:", "icon_biggrin2"}, {":crazy:", "crazy0to"}, {":hooligan:", "hooligan"},
        {":closetema:", "closetema"}, {":disk:", "disk"}, {":jumper:", "jumper"}, {":P:", "be-e-e"}, {":dancer:", "dancer"},
        {":kiss2:", "kiss"}, {":girls_dance:", "girls_dance"}, {":beautiful:", "beautiful"}, {":finest:", "finest"},
        {":modesty:", "modesty"}, {":amazement:", "amazement"}, {":affliction:", "affliction "}, {":stupid:", "stupid"},
        {":coolest:", "coolest"}, {":bis:", "bis"}, {":rose:", "in_love2"}, {":anime_01:", "anime_01"}, {":anime_02:", "anime_02"},
        {":anime_03:", "anime_03"}, {":anime_04:", "anime_04"}, {":anime_05:", "anime_05"}, {":anime_06:", "anime_06"},
        {":dont_ment:", "ext_dont_ment"}, {":gimmefive:", "ext_gimmefive"}, {":mirror:", "ext_mirror"},
        {":skillet:", "ext_skilletgirl"}, {":good:", "good"}, {":ne:", "ne"}, {":kiss3:", "kiss3"}, {":na_metle:", "na_metle"},
        {":lock:", "lock"}, {":boy:", "cupidboy"}, {":girl:", "cupidgirl"}, {":medved:", "medved"}, {":bayan:", "bayan"},
        {":kk:", "kak_kino"}, {":-|", "icon_neutral"}, {":search:", "use_search"}
    };

    m_IconMapFirstParse = {
        {":offtopic:", "tr_offtopic"}, {":oldtimer:", "ppl_oldtimer"}
    };


    ui->setupUi(this);
    ui->webView->settings()->setUserStyleSheetUrl(QUrl("qrc:/styles.css"));
    ui->webView->settings()->setFontFamily(QWebSettings::StandardFont, "Times New Roman");
    ui->webView->settings()->setFontSize(QWebSettings::DefaultFontSize, 16);
    ui->webView->setHtml(GetHTML(content));
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

    //Заменятся первым что бы не было проблем с например - ":offtopic:" и ":o"
    foreach (QString s, m_IconMapFirstParse.keys())
        res = res.replace(s, QString("<img class=\"smile\" src=\"https://static.t-ru.org/smiles/%1.gif\" alt=\"\">").arg(m_IconMapFirstParse[s]));
    foreach (QString s, m_IconMap.keys())
        res = res.replace(s, QString("<img class=\"smile\" src=\"https://static.t-ru.org/smiles/%1.gif\" alt=\"\">").arg(m_IconMap[s]));

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

    QString fin = "<html lang=\"ru\"><head><meta charset=\"windows-1251\"></head><body class=\"\"><div id=\"page_container\"><div class=\"message td2\">" ;
    fin += "<div class=\"post_head\"></div>";
    fin += "<div class=\"post_body\" id=\"p-0000000\">";
    fin += res + "</div></div></div></body></html>";
    return fin;
}
