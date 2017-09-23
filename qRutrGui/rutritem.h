#ifndef TORRENTITEM_H
#define TORRENTITEM_H
#include <QString>
#include <QDateTime>
class RuTrItem
{
public:
    int id;
    QDateTime registered;
    QString size;
    QString title;
    QString content;
    QString TorrentHash;
    int ForumId;
};

#endif // TORRENTITEM_H
