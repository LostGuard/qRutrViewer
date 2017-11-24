#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    ui->idCheckBox->setChecked(Settings::inst()->value(Settings::ColumnIdView, false).toBool());
    ui->forumCheckBox->setChecked(Settings::inst()->value(Settings::ColumnForum, false).toBool());
    ui->sizeCheckBox->setChecked(Settings::inst()->value(Settings::ColumnSize, false).toBool());
    ui->dateCheckBox->setChecked(Settings::inst()->value(Settings::ColumnRegTime, false).toBool());
    ui->hashCheckBox->setChecked(Settings::inst()->value(Settings::ColumnHash, false).toBool());
    
    ui->magnetEdit->setText(Settings::inst()->value(Settings::MagnetURL, Settings::MagnetURLDefVal).toString());
    ui->torrentEdit->setText(Settings::inst()->value(Settings::TorrentURL, Settings::TorrentURLDefVal).toString());
    ui->torrentSiteEdit->setText(Settings::inst()->value(Settings::TorrentSite, Settings::TorrentSiteDefVal).toString());
    
    ui->maxTableRowsEdit->setText(Settings::inst()->value(Settings::MaxTorrentsTableRows, 500).toString());
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_buttonBox_accepted()
{
    Settings::inst()->setValue(Settings::ColumnIdView, ui->idCheckBox->isChecked());
    Settings::inst()->setValue(Settings::ColumnForum, ui->forumCheckBox->isChecked());
    Settings::inst()->setValue(Settings::ColumnSize, ui->sizeCheckBox->isChecked());
    Settings::inst()->setValue(Settings::ColumnRegTime, ui->dateCheckBox->isChecked());
    Settings::inst()->setValue(Settings::ColumnHash, ui->hashCheckBox->isChecked());
    
    Settings::inst()->setValue(Settings::MagnetURL, ui->magnetEdit->text());
    Settings::inst()->setValue(Settings::TorrentURL, ui->torrentEdit->text());
    Settings::inst()->setValue(Settings::TorrentSite, ui->torrentSiteEdit->text());
    
    bool ok;
    ui->maxTableRowsEdit->text().toInt(&ok);
    if(ok)
        Settings::inst()->setValue(Settings::MaxTorrentsTableRows, ui->maxTableRowsEdit->text().toInt());
    
    done(QDialog::Accepted);
}

void SettingsWindow::on_buttonBox_rejected()
{
    close();
}
