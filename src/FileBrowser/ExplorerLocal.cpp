#include "ExplorerLocal.h"

#include <QEvent>
#include <QFileSystemModel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QProgressDialog>
#include <QScrollBar>
#include <QScroller>
#include <QTimer>

#include "../Common.h"
#include "../Config.h"
#include "../File.h"

ExplorerLocal::ExplorerLocal(bool open, QWidget* parent)
    : QListView(parent), Explorer(open)
{
    horizontalScrollBar()->setStyleSheet(Common::getStyleSheet());
    verticalScrollBar()->setStyleSheet(Common::getStyleSheet());

    // Set proper initial icon.
    setWrapping(!Config::getInstance().listViewInBrowser());

    setupList();
}

ExplorerLocal::~ExplorerLocal()
{
    const auto* fileModel{dynamic_cast<QFileSystemModel*>(model())};

    QModelIndex currentIndex = rootIndex();

    QString lastDir = fileModel->fileInfo(currentIndex).absoluteFilePath();
    Config::getInstance().setLastPickedDir(lastDir);
}

void ExplorerLocal::setupList()
{
#ifdef Q_OS_ANDROID
    QScroller* scroller = QScroller::scroller(viewport());

    scroller->grabGesture(viewport(), QScroller::LeftMouseButtonGesture);
#endif

    QObject::connect(this, &ExplorerLocal::clicked, this,
                     &ExplorerLocal::itemWasActivated);

    QObject::connect(this, &ExplorerLocal::doubleClicked, this,
                     &ExplorerLocal::itemWasActivated);

    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setDropIndicatorShown(false);
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setResizeMode(QListView::Adjust);
    setDragDropMode(QAbstractItemView::NoDragDrop);
}

void ExplorerLocal::setPath(QString path)
{
    const auto* fileModel{dynamic_cast<QFileSystemModel*>(model())};

    QString pathToUse{path};
    if (pathToUse.isEmpty())
        pathToUse = Common::rootPath();
    setRootIndex(fileModel->index(pathToUse));
    QModelIndex newRootIndex = fileModel->index(0, 0, rootIndex());
    setCurrentIndex(newRootIndex);
}

void ExplorerLocal::initialize()
{
    auto* fileModel{new QFileSystemModel(this)};

    fileModel->setFilter(QDir::AllEntries | QDir::AllDirs | QDir::NoDot |
                         QDir::System | QDir::Hidden);

    fileModel->setRootPath(Common::rootPath());

    connect(fileModel, SIGNAL(directoryLoaded(QString)), this,
            SLOT(directoryLoaded(QString)));

    setModel(fileModel);

    // Set proper initialization path.
    QString initPath = Config::getInstance().lastPickedDir();
    if (initPath.isEmpty())
        initPath = Common::rootPath();

    setRootIndex(fileModel->index(initPath));
}

bool ExplorerLocal::initialized() { return model() != nullptr; }

void ExplorerLocal::performOperationOnFile(QString filePath)
{
    if (fileIsValid(filePath))
    {
        QFileInfo fileInfo(filePath);
        QString path(fileInfo.canonicalPath());
        QString baseName(fileInfo.completeBaseName());
        QString suffix(fileInfo.suffix());
        QString* content = nullptr;

        if (open_)
        {
            content = new QString(Common::loadFile(filePath));
        }
        else
        {
            if (QFile::exists(filePath))
            {
                QString msg = tr("Overwrite ") + fileInfo.fileName() + "?";
                QMessageBox::StandardButton answer =
                    QMessageBox::question(this, tr("Overwrite"), msg);

                if (answer == QMessageBox::No)
                    return;
            }
            else
            {
                path = File::filePathToPath(filePath);
                baseName = File::filePathToBaseName(filePath);
                suffix = File::filePathToSuffix(filePath);
            }
        }

        File* file{
            new File(Common::SOURCE_LOCAL, path, baseName, suffix, content)};

        emit filePrepared(file);
    }
    else
    {
        QMessageBox::information(this, tr("Error"), tr("Cannot open..."));
    }
}

QListView* ExplorerLocal::getListView() { return this; }

void ExplorerLocal::itemWasActivated(QModelIndex index)
{
    const auto* fileModel{dynamic_cast<QFileSystemModel*>(model())};

    if (currentItem_ != index.data())
    {
        currentItem_ = index.data().toString();
        return;
    }

    currentItem_.clear();

    QString canonicalFilePath(fileModel->fileInfo(index).canonicalFilePath());
    if (fileModel->isDir(index))
    {
        if (index.data().toString() == "..")
        {
            setRootIndex(fileModel->index(canonicalFilePath));
            QModelIndex newRootIndex = fileModel->index(0, 0, rootIndex());
            setCurrentIndex(newRootIndex);
        }
        else
        {
            if (directoryIsAccessible(canonicalFilePath))
                setRootIndex(index);
        }

        emit pathChanged(getCurrentPath());
    }
    else
    {
        performOperationOnFile(canonicalFilePath);
    }
}

void ExplorerLocal::directoryLoaded(const QString& path)
{
    QString oldCurrentPath = getCurrentPath();

    directoryIsAccessible(path);

    QString newCurrentPath = getCurrentPath();
    if (oldCurrentPath != newCurrentPath)
        emit pathChanged(newCurrentPath);
}

bool ExplorerLocal::directoryIsAccessible(const QString& path)
{
    const auto* fileModel{dynamic_cast<QFileSystemModel*>(model())};

    if (QDir newDir(path); newDir.entryList(QDir::AllEntries).count() == 0)
    {
        QMessageBox::information(this, tr("Error"), tr("Not accessible..."));

        if (newDir.cdUp())
        {
            setRootIndex(fileModel->index(newDir.absolutePath()));
        }
        else
        {
            QMessageBox::information(this, tr("Error"), tr("I/O error..."));
            setRootIndex(fileModel->index(""));
        }

        return false;
    }

    return true;
}

void ExplorerLocal::listViewItemClicked(const QModelIndex& index)
{
    const auto* fileModel{dynamic_cast<QFileSystemModel*>(model())};

    if (!fileModel->isDir(index))
        emit pathChanged(fileModel->filePath(index));
}

void ExplorerLocal::mouseMoveEvent(QMouseEvent* event) { event->accept(); }

QString ExplorerLocal::getCurrentPath()
{
    const auto* fileModel{dynamic_cast<QFileSystemModel*>(model())};

    QString path{fileModel->fileInfo(rootIndex()).canonicalFilePath()};

    return path;
}

bool ExplorerLocal::fileIsValid(QString file)
{
    QFileInfo fileInfo(file);

    if (open_)
        return (QFile::exists(file) && fileInfo.isFile() &&
                fileInfo.isReadable());

    if (QFile::exists(file))
        return (fileInfo.isFile() && fileInfo.isWritable());

    bool dirExists{fileInfo.dir().exists()};
    bool writable{QFileInfo(fileInfo.dir().canonicalPath()).isWritable()};
    return (dirExists && writable);
}

bool ExplorerLocal::isWrappingContent() { return QListView::isWrapping(); }
