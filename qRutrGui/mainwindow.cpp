#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QClipboard>

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

    CategoryDelegate *delegate = new CategoryDelegate();
    ui->tableView->setItemDelegateForColumn(0, delegate);
    ui->tableView->setModel(m_Model);
    
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), SLOT(slotTableCustomMenuRequested(QPoint)));
    
    //ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->hide();
    connect(ui->searchEdit, SIGNAL(returnPressed()), this, SLOT(on_searchButton_clicked()));
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
    QStringList *kwlist = new QStringList();
    *kwlist = ui->searchEdit->text().split(" ");
    if (!ui->searchEdit->text().isEmpty())
    {
        QMovie *movie = new QMovie(":/icons/index.ajax-spinner-preloader.gif");
        m_progressLabel = new QLabel(this);

        m_progressLabel->setAttribute(Qt::WA_DeleteOnClose);
        m_progressLabel->setWindowFlags(Qt::SplashScreen);
        m_progressLabel->setAutoFillBackground(false);
        m_progressLabel->setAttribute(Qt::WA_TranslucentBackground);
        m_progressLabel->setAttribute(Qt::WA_NoSystemBackground);
        this->setEnabled(false);
        //progressLabel->setGeometry(this->geometry().topLeft().x()+20, this->geometry().topLeft().y()+160, 550, 350);
        //progressLabel->setFixedHeight(350);
        //progressLabel->setFixedWidth(550);

        m_progressLabel->setMovie(movie);
        movie->start();
        m_progressLabel->show();

        DataBaseWorker *wk = new DataBaseWorker(m_db, kwlist, 0, m_MaxItemsView, ui->categoryComboBox->currentData().toInt());
        connect(wk, SIGNAL(signalSearchFinished(QList<RuTrItem*>*,int)), m_Model, SLOT(slotSearchFinished(QList<RuTrItem*>*)));
        connect(wk, SIGNAL(signalSearchFinished(QList<RuTrItem*>*,int)), this, SLOT(slotSearchFinished()));
        connect(wk, SIGNAL(finished()), wk, SLOT(deleteLater()));
        wk->start();
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

void MainWindow::slotTableCustomMenuRequested(QPoint pos)
{
    //QModelIndex index = ui->tableView->indexAt(pos);
    if (ui->tableView->selectionModel()->selectedIndexes().count() > 0)
    {
        QMenu* menu = new QMenu(this);
        menu->addAction(ui->actionView);
        menu->addAction(ui->actionCopy);
        menu->addAction(ui->actionCopyFull);
        menu->addAction(ui->actionCopyHash);
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}

void MainWindow::slotSearchFinished()
{
    m_progressLabel->close();
    this->setEnabled(true);
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


void MainWindow::on_actionView_triggered()
{
    slotShowItem();
}

void MainWindow::on_actionCopy_triggered()
{
    QString res = "";
    QModelIndexList mlist = ui->tableView->selectionModel()->selectedIndexes();
    for (int i = 0; i < mlist.length(); ++i)
    {
        RuTrItem *item = m_Model->getItem(mlist[i].row());
        res += item->title + "\n";
    }
    QApplication::clipboard()->setText(res);
}

void MainWindow::on_actionCopyFull_triggered()
{
    QString res = "";
    QModelIndexList mlist = ui->tableView->selectionModel()->selectedIndexes();
    for (int i = 0; i < mlist.length(); ++i)
    {
        RuTrItem *item = m_Model->getItem(mlist[i].row());
        res += item->title + "\n";
        res += item->registered.toString("yyyy.MM.dd hh:mm:ss") + "\n";
        res += item->size + "\n";
        res += item->TorrentHash + "\n";
        res += "id: " + QString::number(item->id) + "\n";
        res += "forumId: " + QString::number(item->ForumId) + "\n";
        res += "-----------------------";
    }
    QApplication::clipboard()->setText(res);
}

void MainWindow::on_actionCopyHash_triggered()
{
    QString res = "";
    QModelIndexList mlist = ui->tableView->selectionModel()->selectedIndexes();
    for (int i = 0; i < mlist.length(); ++i)
    {
        RuTrItem *item = m_Model->getItem(mlist[i].row());
        res += item->TorrentHash + "\n";
    }
    QApplication::clipboard()->setText(res);
}
