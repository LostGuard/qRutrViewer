#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_db = new DataBase();

    if(!m_db->init(false))
    {
        QMessageBox::critical(this, "Ошибка инициализации БД", m_db->errorString);
        return;
    }

    ui->categoryComboBox->addItem(" - ", -1);
    QMap<int, QString> cmap = m_db->getCategories();
    foreach (int i, cmap.keys())
    {
        ui->categoryComboBox->addItem(cmap[i], i);
    }

    m_Model = new TableModel(this);
    connect(this, SIGNAL(signalSearchFinished(QList<RuTrItem*>*)), m_Model, SLOT(slotSearchFinished(QList<RuTrItem*>*)));
    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotShowItem()));

    ui->tableView->setModel(m_Model);
    //ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_searchButton_clicked()
{
    m_SearchStartIndex = 0;
    startSearch();
}

void MainWindow::startSearch()
{
    QString cmd;
    QList<RuTrItem*>* items = new QList<RuTrItem*>();
    QStringList kwlist = ui->searchEdit->text().split(" ");
    if (!ui->searchEdit->text().isEmpty())
    {
        m_db->Search(items, kwlist, m_SearchStartIndex, m_SearchStartIndex + m_MaxItemsView - 1,
                     ui->categoryComboBox->currentData().toInt());
        emit signalSearchFinished(items);
    }
}

void MainWindow::slotShowItem()
{
    QModelIndexList mlist = ui->tableView->selectionModel()->selectedIndexes();
    if (mlist.length() > 0)
    {
        ItemViewForm* frm = new ItemViewForm(m_db->GetContent(m_Model->getItem(mlist[0].row())));
        frm->show();
    }
}

void MainWindow::on_prevButton_clicked()
{
    if (!ui->searchEdit->text().isEmpty() && m_SearchStartIndex >= m_MaxItemsView)
    {
        m_SearchStartIndex -= m_MaxItemsView;
        startSearch();
    }
}

void MainWindow::on_nextButton_clicked()
{
    if (!ui->searchEdit->text().isEmpty() && ui->tableView->model()->rowCount() == m_MaxItemsView)
    {
        m_SearchStartIndex += m_MaxItemsView;
        startSearch();
    }
}

