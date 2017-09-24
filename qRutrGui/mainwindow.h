#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QLabel>
#include <QMovie>

#include "tablemodel.h"
#include "itemviewform.h"
#include "../qRutrXMLConv/database.h"
#include "categorydelegate.h"
#include "databaseworker.h"

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
    void slotShowItem();
    void slotTableCustomMenuRequested(QPoint);
    void slotSearchFinished();

    void on_prevButton_clicked();
    void on_nextButton_clicked();
    void on_actionView_triggered();
    void on_actionCopy_triggered();
    void on_actionCopyFull_triggered();
    void on_actionCopyHash_triggered();
    
signals:
    void signalSearchFinished(QList<RuTrItem*>* items);

private:
    Ui::MainWindow *ui;
    DataBase* m_db;
    TableModel *m_Model;
    int m_SearchStartIndex = 0;
    int m_MaxItemsView = 500;
    QLabel *m_progressLabel;
    void startSearch();
};

#endif // MAINWINDOW_H
