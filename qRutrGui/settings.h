#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings: public QSettings
{
public:

    static const QString WindowState;
    static const QString WindowIsFull;
    static const QString ColumnIdView;
    static const QString ColumnForum;
    static const QString ColumnSize;
    static const QString ColumnRegTime;
    static const QString ColumnHash;
    
    static const QString MagnetURL;
    static const QString TorrentURL;
    static const QString TorrentSite;
    
    static const QString MagnetURLDefVal;
    static const QString TorrentURLDefVal;
    static const QString TorrentSiteDefVal;
    
    static const QString MaxTorrentsTableRows;

    static Settings *inst();
    
private:
    Settings(QString, QSettings::Format);
};

#endif // SETTINGS_H
