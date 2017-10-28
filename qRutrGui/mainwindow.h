#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QLabel>
#include <QMovie>
#include <QCloseEvent>

#include "tablemodel.h"
#include "itemviewform.h"
#include "../qRutrXMLConv/database.h"
#include "databaseworker.h"
#include "categorydelegate.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_searchButton_clicked();
    void slotShowItem(RuTrItem* item, QString content);
    void slotTableCustomMenuRequested(QPoint);
    void slotViewError(QString err);

    void on_prevButton_clicked();
    void on_nextButton_clicked();
    void on_actionView_triggered();
    void on_actionCopy_triggered();
    void on_actionCopyFull_triggered();
    void on_actionCopyHash_triggered();
    void on_actionSearchInCategory_triggered();

signals:
    void signalCloseAll();


private:
    Ui::MainWindow *ui;
    DataBase* m_db;
    TableModel *m_Model;
    int m_SearchStartIndex = 0;
    int m_MaxItemsView = 500;
    QLabel *m_progressLabel;
    QMap<int, QString> m_categoryMap;

    void startSearch();
    void closeEvent(QCloseEvent *event);
    void updateCategories(QString filter);
private slots:
    void slotFreezeInterface();
    void slotUnfreezeInterface();
    void on_actionCopyMagnet_triggered();
    void on_actionGetTorrent_triggered();
    void on_actionCopyRutrackerURL_triggered();
    void on_categorySearchEdit_textChanged(const QString &arg1);
};

#endif // MAINWINDOW_H
