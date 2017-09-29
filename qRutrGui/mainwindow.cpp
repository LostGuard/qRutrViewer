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

    QList<int> sizes;
    int h = ui->splitter->sizeHint().height() / 7;
    sizes << h  << ui->splitter->sizeHint().height() - h;
    ui->splitter->setSizes(sizes);

    QListWidgetItem *litem = new QListWidgetItem();
    litem->setText(" - ");
    litem->setData(Qt::UserRole, -1);
    ui->categoryListWidget->insertItem(ui->categoryListWidget->count(), litem);
    ui->categoryListWidget->setCurrentRow(0);
    QMap<int, QString> cmap = m_db->getCategories();
    foreach (int i, cmap.keys())
    {
        litem = new QListWidgetItem();
        litem->setText(cmap[i]);
        litem->setData(Qt::UserRole, i);
        ui->categoryListWidget->insertItem(ui->categoryListWidget->count(), litem);
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
    slotFreezeInterface();

    QString cmd;
    QList<RuTrItem*>* items = new QList<RuTrItem*>();

    int catId = -1;
    DataBaseWorker *wk = new DataBaseWorker(m_db);
    if (ui->categoryListWidget->currentItem())
        catId = ui->categoryListWidget->currentItem()->data(Qt::UserRole).toInt();
    wk->SetSearchWorker(ui->searchEdit->text(), m_SearchStartIndex, m_MaxItemsView, catId, !ui->detailedSearchCheckBox->isChecked());
    connect(wk, SIGNAL(signalSearchFinished(QList<RuTrItem*>*,int)), m_Model, SLOT(slotSearchFinished(QList<RuTrItem*>*)));
    connect(wk, SIGNAL(signalSearchFinished(QList<RuTrItem*>*,int)), this, SLOT(slotUnfreezeInterface()));
    connect(wk, SIGNAL(signalError(QString)), this, SLOT(slotViewError(QString)));
    connect(wk, SIGNAL(finished()), wk, SLOT(deleteLater()));
    wk->start();
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
    connect(this, SIGNAL(signalCloseAll()), frm, SLOT(close()));
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
    if (m_SearchStartIndex >= m_MaxItemsView)
    {
        m_SearchStartIndex -= m_MaxItemsView;
        startSearch();
    }
}

void MainWindow::on_nextButton_clicked()
{
    if (ui->tableView->model()->rowCount() >= m_MaxItemsView)
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
    QModelIndexList mlist = ui->tableView->selectionModel()->selectedRows();
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
        for (int i = 0; i < ui->categoryListWidget->count(); ++i)
            if (ui->categoryListWidget->item(i)->data(Qt::UserRole).toInt() == item->ForumId)
                ui->categoryListWidget->setCurrentRow(i);
        on_searchButton_clicked();
    }

}

void MainWindow::closeEvent(QCloseEvent *event)
{
     emit signalCloseAll();
     event->accept();
}
