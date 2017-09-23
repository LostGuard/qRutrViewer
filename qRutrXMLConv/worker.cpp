#include "worker.h"

Worker::Worker(DataBase *idb, QFile *iXMLfile)
{
    m_db = idb;
    m_XMLfile = iXMLfile;
}

Worker::~Worker()
{
    delete m_db;
    m_XMLfile->close();
    delete m_XMLfile;
}

void Worker::run()
{
    emit signalAppendToLog("Запускаем процесс:");
    QTime time = QTime::currentTime();
    RuTrItem* item;
    m_count = 0;
    QXmlStreamReader xmlReader(m_XMLfile);

    QMap<int, QString> mapforums;

    while(!xmlReader.atEnd())
    {

        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if (token == QXmlStreamReader::StartElement && xmlReader.name() == "torrent")
        {
            if (m_stop)
                break;

            ++m_count;
            item = new RuTrItem();
            item->id = xmlReader.attributes().value("id").toULong();
            item->size = xmlReader.attributes().value("size").toString();
            item->registered = QDateTime::fromString(xmlReader.attributes().value("registred_at").toString(),"yyyy.MM.dd hh:mm:ss");
            int flag = 0;
            while (true)
            {
                token = xmlReader.readNext();
                if (xmlReader.hasError())
                {
                    emit signalAppendToLog("Ошибка при чтении xml: " + xmlReader.errorString());
                    return;
                }

                if (token == QXmlStreamReader::StartElement && xmlReader.name() == "title")
                {
                    xmlReader.readNext();
                    item->title = xmlReader.text().toString();
                }
                else if (token == QXmlStreamReader::StartElement && xmlReader.name() == "content")
                {
                    xmlReader.readNext();
                    item->content = xmlReader.text().toString();
                }
                else if (token == QXmlStreamReader::StartElement && xmlReader.name() == "torrent")
                {
                    ++flag;
                    item->TorrentHash = xmlReader.attributes().value("hash").toString();
                }
                else if (token == QXmlStreamReader::StartElement && xmlReader.name() == "forum")
                {
                    int id = xmlReader.attributes().value("id").toInt();
                    item->ForumId = id;
                    xmlReader.readNext();
                    if (!mapforums.contains(id))
                        mapforums.insert(id, xmlReader.text().toString());
                }
                else if (token == QXmlStreamReader::EndElement && xmlReader.name() == "torrent")
                {
                    if (flag <= 0)
                    {
                        m_db->saveRuTrItemCached(item);
                        break;
                    }
                    else
                    {
                        --flag;
                    }
                }
            }
        }
    }
    m_db->sync();
    m_db->saveForums(mapforums);
    if (m_stop)
        emit signalAppendToLog("Парсинг был прерван.");
    emit signalAppendToLog("Обработано записей: " + QString::number(m_count));
    emit signalAppendToLog("Затраченое время(ms):" + QString::number(time.elapsed()));
}

void Worker::slotStopParse()
{
    m_stop = true;
}

void Worker::slotGetStatus()
{
    emit signalAppendToLog("Обработано записей: " + QString::number(m_count));
}
