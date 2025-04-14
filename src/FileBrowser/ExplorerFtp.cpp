#include "ExplorerFtp.h"

#include <QApplication>
#include <QFileIconProvider>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>
#include <QScrollEvent>
#include <QScroller>

#include "../Common.h"
#include "../Config.h"
#include "../File.h"

ExplorerFtp::ExplorerFtp(bool open, QWidget* parent)
    : QListWidget(parent), Explorer(open)
{
    horizontalScrollBar()->setStyleSheet(Common::getStyleSheet());
    verticalScrollBar()->setStyleSheet(Common::getStyleSheet());

    // Set proper initial icon.
    setWrapping(!Config::getInstance().listViewInBrowser());

    setSortingEnabled(true);

    progressDialog_.setVisible(false);

    progressDialog_.setRange(0, 0);
    // QTBUG-47042
    progressDialog_.reset();

    connect(&progressDialog_, SIGNAL(canceled()), this, SLOT(cancelDownload()));

    setupList(this);
}

ExplorerFtp::~ExplorerFtp()
{
#ifdef FTP
    if (ftp_)
        delete ftp_;
#endif
}

void ExplorerFtp::setPath(QString path)
{
    if (path.isEmpty())
        path = Common::rootPath();

    if (currentPath_ == path)
        return;

    Common::centerWidget(this, &progressDialog_);
    progressDialog_.setVisible(true);

    newPath_ = path;

#ifdef FTP
    ftp_->cd(path);
#endif
}

QString ExplorerFtp::getCurrentPath() { return currentPath_; }

bool ExplorerFtp::fileIsValid(QString /*file*/) { return false; }

bool ExplorerFtp::isWrapping() { return QListWidget::isWrapping(); }

void ExplorerFtp::setWrapping(bool wrapping)
{
    QListWidget::setWrapping(wrapping);
}

void ExplorerFtp::initialize()
{
#ifdef FTP
    ftp_ = new QFtp();

    connect(ftp_, SIGNAL(commandFinished(int, bool)), this,
            SLOT(ftpCommandFinished(int, bool)));
    connect(ftp_, SIGNAL(listInfo(QUrlInfo)), this, SLOT(addToList(QUrlInfo)));
    connect(ftp_, SIGNAL(dataTransferProgress(qint64, qint64)), this,
            SLOT(updateDataTransferProgress(qint64, qint64)));

    ftp_->setTransferMode(QFtp::Passive);
    ftp_->connectToHost(Config::getInstance().ftpHost());
    QString login = Config::getInstance().ftpLogin();
    QString password = Config::getInstance().ftpPassword();
    ftp_->login(login, password);
#endif
}

bool ExplorerFtp::initialized() { return ftp_ != nullptr; }

void ExplorerFtp::performOperationOnFile(QString filePath)
{
    // TODO Check if item exist and than perform correct action.

    if (open_)
    {
        fileBuffer_.open(QIODevice::ReadWrite);
#ifdef FTP
        ftp_->get(filePath, &fileBuffer_);
#endif
    }
    else
    {
        QString fileName = File::filePathToFileName(filePath);
        QList<QListWidgetItem*> items =
            findItems(fileName, Qt::MatchCaseSensitive | Qt::MatchFixedString);

        if (1 != items.size())
        {
            Q_ASSERT(false);
            return;
        }

        Item* item = static_cast<Item*>(items.front());

        QString baseName = File::fileNameToBaseName(item->text());
        QString suffix = File::fileNameToSuffix(item->text());
        File* file = new File(Common::SOURCE_FTP, currentPath_, baseName,
                              suffix, nullptr);

#ifdef FTP
        ftp_->close();
#endif

        emit filePrepared(file);
    }
}

void ExplorerFtp::resizeEvent(QResizeEvent* event)
{
    QListWidget::resizeEvent(event);
    Common::centerWidget(this, &progressDialog_);
}

QListView* ExplorerFtp::getListView() { return this; }

void ExplorerFtp::listViewItemClicked(const QModelIndex& index)
{
    Item* itemClicked = static_cast<Item*>(item(index.row()));

    if (!itemClicked->isDir())
    {
        QString pathToUse = getPathToUse(itemClicked);

        emit pathChanged(pathToUse);
    }
}

void ExplorerFtp::ftpCommandFinished(int, [[maybe_unused]] bool error)
{
#ifdef FTP
    switch (ftp_->currentCommand())
    {
        case QFtp::ConnectToHost:
        {
            if (true == error)
            {
                QMessageBox::information(
                    this, tr("FTP"),
                    tr("Unable to connect to the FTP server.\n"
                       "Error: %1")
                        .arg(ftp_->errorString()));

                return;
            }

            break;
        }

        case QFtp::Login:
        {
            if (true == error)
            {
                QMessageBox::information(
                    this, tr("FTP"),
                    tr("Unable to login. Wrong user or password.\n"
                       "Error: %1")
                        .arg(ftp_->errorString()));
                return;
            }

            if (true == currentPath_.isEmpty())
            {
                newPath_ = Common::rootPath();
                ftp_->cd(Common::rootPath());
            }
            else
            {
                ftp_->cd(currentPath_);
            }
            break;
        }

        case QFtp::Cd:
        {
            itemsList_.clear();
            ftp_->list();
            break;
        }

        case QFtp::List:
        {
            progressDialog_.setVisible(false);
            clear();

            // Change path successful. Change current path.
            currentPath_ = newPath_;
            if (false == currentPath_.isEmpty() &&
                currentPath_ != Common::rootPath())
            {
                Item* item = new Item("..", true, true, true);
                addItem(item);
            }

            foreach (Item* itemOnlist, itemsList_)
            {
                addItem(itemOnlist);
            }

            emit pathChanged(currentPath_);

            break;
        }

        case QFtp::Get:
        {
            progressDialog_.setVisible(false);
            progressDialog_.setRange(0, 0);
            // QTBUG-47042
            progressDialog_.reset();

            Item* item = static_cast<Item*>(currentItem());
            QString baseName = File::fileNameToBaseName(item->text());
            QString suffix = File::fileNameToSuffix(item->text());

            File* file = new File(Common::SOURCE_FTP, currentPath_, baseName,
                                  suffix, new QString(fileBuffer_.buffer()));
            ftp_->close();
            emit filePrepared(file);
            break;
        }

        default:
        {
            break;
        }
    }
#endif
}

QString ExplorerFtp::getPathToUse(const Item* itemClicked) const
{
    QString pathToUse = currentPath_;
    if ((!pathToUse.isEmpty()) && (pathToUse != Common::rootPath()))
        pathToUse.append("/");

    pathToUse.append(itemClicked->text());

    return pathToUse;
}

void ExplorerFtp::itemActivated(QModelIndex index)
{
    auto* itemClicked{dynamic_cast<Item*>(item(index.row()))};

    if (!itemClicked->readable())
    {
        QMessageBox::information(
            this, tr("FTP"),
            tr("No permissions to open: ") + itemClicked->text());
        return;
    }

    // Do "cd ..".
    if (itemClicked->text() == "..")
    {
        Common::centerWidget(this, &progressDialog_);
        progressDialog_.setVisible(true);

        newPath_ = currentPath_.left(currentPath_.lastIndexOf('/'));
        if (newPath_.isEmpty())
        {
            newPath_ = Common::rootPath();
#ifdef FTP
            ftp_->cd(Common::rootPath());
#endif
        }
        else
        {
#ifdef FTP
            ftp_->cd(newPath_);
#endif
        }

        return;
    }

    QString pathToUse = getPathToUse(itemClicked);
    if (itemClicked->isDir())
    {
        setPath(pathToUse);
    }
    else
    {
        performOperationOnFile(pathToUse);
    }
}

#ifdef FTP
void ExplorerFtp::addToList(const QUrlInfo& urlInfo)
{
    // Ignore returned by server ".." and ".".
    if (".." == urlInfo.name() || "." == urlInfo.name())
    {
        return;
    }

    itemsList_.append(new Item(urlInfo.name(), urlInfo.isDir(),
                               urlInfo.isReadable(), urlInfo.isWritable()));
}
#endif

void ExplorerFtp::updateDataTransferProgress(qint64 readBytes,
                                             qint64 totalBytes)
{
    if (!progressDialog_.isVisible())
    {
        Common::centerWidget(this, &progressDialog_);
        progressDialog_.setVisible(true);
    }
    progressDialog_.setMaximum(totalBytes);
    progressDialog_.setValue(readBytes);
}

void ExplorerFtp::cancelDownload()
{
#ifdef FTP
    ftp_->abort();
    ftp_->clearPendingCommands();
    ftp_->deleteLater();
#endif

    // Recreate QFtp (abort is not working as it should).
    initialize();

    // Disable all items.
    for (int i = 0; i < count(); ++i)
    {
        QListWidgetItem* currentItem = item(i);
        currentItem->setFlags(currentItem->flags() & ~Qt::ItemIsEnabled);
    }

    newPath_ = currentPath_;
}

ExplorerFtp::Item::Item(const QString& text, bool dir, bool readable,
                        bool writeable)
    : QListWidgetItem(text), readable_(readable), writeable_(writeable)
{
    const static QFileIconProvider iconsProvider;
    if (dir)
        setIcon(iconsProvider.icon(QFileIconProvider::Folder));
    else
        setIcon(iconsProvider.icon(QFileIconProvider::File));

    setData(Qt::UserRole, QVariant(dir));

    // Italic for items without permissions.
    if (!readable_)
    {
        QFont itemFont(font());
        itemFont.setItalic(true);
        setFont(itemFont);
    }
}

bool ExplorerFtp::Item::operator<(const QListWidgetItem& other) const
{
    bool otherIsDir = other.data(Qt::UserRole).toBool();
    bool currentIsDir = isDir();
    if ((otherIsDir && currentIsDir) || (!otherIsDir && !currentIsDir))
        return text() < other.text();

    return currentIsDir;
}

bool ExplorerFtp::Item::isDir() const { return data(Qt::UserRole).toBool(); }

bool ExplorerFtp::Item::writeable() const { return writeable_; }

bool ExplorerFtp::Item::readable() const { return readable_; }
