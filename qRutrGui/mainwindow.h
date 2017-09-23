#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "tablemodel.h"
#include "itemviewform.h"
#include "../qRutrXMLConv/database.h"

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

    void on_prevButton_clicked();

    void on_nextButton_clicked();

signals:
    void signalSearchFinished(QList<RuTrItem*>* items);

private:
    Ui::MainWindow *ui;
    DataBase* m_db;
    TableModel *m_Model;
    int m_SearchStartIndex = 0;
    int m_MaxItemsView = 500;

    void startSearch();
};

#endif // MAINWINDOW_H
