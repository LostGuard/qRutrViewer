#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFile>
#include <QThread>
#include <QTimer>


#include "database.h"
#include "worker.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_XMLButton_clicked();
    void on_dbButton_clicked();
    void on_startButton_clicked();
    void slotXMLParseFinished();
public slots:
    void slotAppendToLog(QString msg);
private:
    Ui::Widget *ui;
    Worker *m_worker;
    bool m_isStarted;
    QTimer* m_timer;

signals:
    void signalAppendToLog(QString msg);
    void signalStopParse();
};

#endif // WIDGET_H
