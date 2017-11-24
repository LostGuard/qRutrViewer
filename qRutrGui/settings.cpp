#include "settings.h"

const QString Settings::WindowState = "WindowState";
const QString Settings::WindowIsFull = "WindowIsFull";
const QString Settings::ColumnIdView = "ColumnIdView";
const QString Settings::ColumnForum = "ColumnForum";
const QString Settings::ColumnSize = "ColumnSize";
const QString Settings::ColumnRegTime = "ColumnRegTime";
const QString Settings::ColumnHash = "ColumnHash";

const QString Settings::MagnetURL = "MagnetURL";
const QString Settings::TorrentURL = "TorrentURL";
const QString Settings::TorrentSite = "TorrentSite";

const QString Settings::MagnetURLDefVal = "magnet:?xt=urn:btih:#1";
const QString Settings::TorrentURLDefVal = "http://itorrents.org/torrent/#1.torrent";
const QString Settings::TorrentSiteDefVal = "https://rutracker.org/forum/viewtopic.php?t=#2";

const QString Settings::MaxTorrentsTableRows = "MaxTorrentsTableRows";

Settings *Settings::inst()
{
    static Settings *inst = new Settings("config.ini", QSettings::IniFormat);
    inst->setIniCodec("UTF-8");
    return inst;
}

Settings::Settings(QString path, QSettings::Format format)
    :QSettings(path, format)
{
}
