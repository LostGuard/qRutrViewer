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

    //progressLabel->setGeometry(this->geometry().topLeft().x()+20, this->geometry().topLeft().y()+160, 550, 350);
    //progressLabel->setFixedHeight(350);
    //progressLabel->setFixedWidth(550);
    QMovie *movie = new QMovie(":/icons/index.ajax-spinner-preloader.gif");
    m_progressLabel = new QLabel(this);
    //m_progressLabel->setAttribute(Qt::WA_DeleteOnClose);
    m_progressLabel->setWindowFlags(Qt::SplashScreen);
    m_progressLabel->setAutoFillBackground(false);
    m_progressLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_progressLabel->setAttribute(Qt::WA_NoSystemBackground);
    m_progressLabel->setMovie(movie);
    movie->start();

    m_Model = new TableModel(this, cmap);
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_actionView_triggered()));

    ui->tableView->setModel(m_Model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), SLOT(slotTableCustomMenuRequested(QPoint)));
    CategoryDelegate* delegate = new CategoryDelegate();
    ui->tableView->setItemDelegateForColumn(0, delegate);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    connect(this->ui->tableView->horizontalHeader(), SIGNAL(sectionResized(int,int,int)),
            ui->tableView, SLOT(resizeRowsToContents()));

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
        this->setEnabled(false);

        DataBaseWorker *wk = new DataBaseWorker(m_db);
        wk->SetSearchWorker(kwlist, 0, m_MaxItemsView, ui->categoryComboBox->currentData().toInt());
        connect(wk, SIGNAL(signalSearchFinished(QList<RuTrItem*>*,int)), m_Model, SLOT(slotSearchFinished(QList<RuTrItem*>*)));
        connect(wk, SIGNAL(signalSearchFinished(QList<RuTrItem*>*,int)), this, SLOT(slotUnfreezeInterface()));
        connect(wk, SIGNAL(signalError(QString)), this, SLOT(slotViewError(QString)));
        connect(wk, SIGNAL(finished()), wk, SLOT(deleteLater()));
        wk->start();
    }
}

void MainWindow::slotFreezeInterface()
{
    this->setEnabled(false);
    m_progressLabel->show();
}

void MainWindow::slotUnfreezeInterface()
{
    this->setEnabled(true);
    m_progressLabel->hide();
}

void MainWindow::slotShowItem(RuTrItem* item, QString content)
{
    ItemViewForm* frm = new ItemViewForm(content);
    frm->show();
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
        menu->addAction(ui->actionSearchInCategory);
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}

void MainWindow::slotViewError(QString err)
{
    QMessageBox::warning(this, "Ошибка", err);
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
    QModelIndexList mlist = ui->tableView->selectionModel()->selectedIndexes();
    if (mlist.length() > 0)
    {
        DataBaseWorker *dw = new DataBaseWorker(m_db);
        dw->SetRequestContentWorker(m_Model->getItem(mlist[0].row()));
        connect(dw, SIGNAL(signalGetContentFinished(RuTrItem*,QString)), this, SLOT(slotShowItem(RuTrItem*,QString)));
        connect(dw, SIGNAL(signalGetContentFinished(RuTrItem*,QString)), this, SLOT(slotUnfreezeInterface()));
        slotFreezeInterface();
        dw->start();
    }
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
        res += "forum: " + m_Model->getCategoryText(item->ForumId) + "\n";
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

void MainWindow::on_actionSearchInCategory_triggered()
{
    QModelIndexList mlist = ui->tableView->selectionModel()->selectedIndexes();
    if (mlist.length() > 0)
    {
        RuTrItem *item = m_Model->getItem(mlist[0].row());
        ui->categoryComboBox->setCurrentText(m_Model->getCategoryText(item->ForumId));
        on_searchButton_clicked();
    }

}
