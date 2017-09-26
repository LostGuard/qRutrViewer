#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QXmlStreamReader>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(this, SIGNAL(signalAppendToLog(QString)), this, SLOT(slotAppendToLog(QString)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_XMLButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Выберите файл данных XML", QString(), "*.xml");
    ui->XMLEdit->setText(path);
}

void Widget::on_dbButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Укажите папку где будет лежать база");
    ui->dbEdit->setText(path);
}

void Widget::on_startButton_clicked()
{
    if (m_isStarted)
    {
        ui->startButton->setText("Останавливается");
        emit signalStopParse();
        return;
    }

    QFile* file = new QFile(ui->XMLEdit->text());
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Ошибка инициализации", "Невозможно открыть XML файл");
        return;
    }

    //DataBase* db = new DataBase("/home/lin2/ssd/");
    bool fastDel = true;
    DataBase* db = new DataBase(ui->dbEdit->text());
    if (ui->isTestCheckBox->isChecked())
    {
        db->setWriteMode(false, false);
        fastDel = false;
    }
    else
        db->setWriteMode(ui->baseDataCheckBox->isChecked(), ui->contentCheckBox->isChecked());

    if (!db->init(fastDel))
    {
        QMessageBox::critical(this, "Ошибка инициализации базы", db->errorString);
        return;
    }
    db->resetData();

    m_timer = new QTimer();
    m_timer->setInterval(2000);

    m_worker = new Worker(db, file);
    connect(m_worker, SIGNAL(signalAppendToLog(QString)), this, SLOT(slotAppendToLog(QString)));
    connect(m_worker, SIGNAL(finished()), this, SLOT(slotXMLParseFinished()));
    connect(this, SIGNAL(signalStopParse()), m_worker, SLOT(slotStopParse()));
    connect(m_timer, SIGNAL(timeout()), m_worker, SLOT(slotGetStatus()));
    m_worker->start();
    m_isStarted = true;
    m_timer->start();
    ui->startButton->setText("Остановить");
}

void Widget::slotXMLParseFinished()
{
    ui->startButton->setText("Начать");
    m_isStarted = false;
    delete m_worker;
    m_timer->stop();
    delete m_timer;
}

void Widget::slotAppendToLog(QString msg)
{
    ui->logTextEdit->appendPlainText(msg);
}
