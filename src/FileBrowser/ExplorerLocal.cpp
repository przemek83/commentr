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

ExplorerLocal::ExplorerLocal(bool open, Config& config, QWidget* parent)
    : QListView(parent), Explorer(open), config_(config)
{
    horizontalScrollBar()->setStyleSheet(Common::getStyleSheet());
    verticalScrollBar()->setStyleSheet(Common::getStyleSheet());

    // Set proper initial icon.
    setWrapping(!config_.listViewInBrowser());

    setupList();
}

ExplorerLocal::~ExplorerLocal()
{
    const auto* fileModel{dynamic_cast<QFileSystemModel*>(model())};

    QModelIndex currentIndex = rootIndex();

    QString lastDir = fileModel->fileInfo(currentIndex).absoluteFilePath();
    config_.setLastPickedDir(lastDir);
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

    connect(fileModel, &QFileSystemModel::directoryLoaded, this,
            &ExplorerLocal::directoryLoaded);

    setModel(fileModel);

    // Set proper initialization path.
    QString initPath = config_.lastPickedDir();
    if (initPath.isEmpty())
        initPath = Common::rootPath();

    setRootIndex(fileModel->index(initPath));
}

bool ExplorerLocal::initialized() { return model() != nullptr; }

void ExplorerLocal::performOperationOnFile(QString filePath)
{
    if (!fileIsValid(filePath))
    {
        QMessageBox::information(this, tr("Error"), tr("Cannot open file..."));
        return;
    }

    QFileInfo fileInfo(filePath);
    QString filePathToUse{fileInfo.filePath()};
    QString content;

    if (isOpen())
    {
        content = Common::loadFile(filePath);
    }
    else
    {
        if (QFile::exists(filePath))
        {
            QString msg =
                tr("Overwrite ") + fileInfo.fileName() + QStringLiteral("?");
            QMessageBox::StandardButton answer =
                QMessageBox::question(this, tr("Overwrite"), msg);

            if (answer == QMessageBox::No)
                return;
        }
    }

    File file(Common::Source::LOCAL, filePathToUse, std::move(content));

    emit filePrepared(file);
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
        if (index.data().toString() == QLatin1String(".."))
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
            setRootIndex(fileModel->index(QLatin1String("")));
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

void ExplorerLocal::mouseMoveEvent(QMouseEvent* e) { e->accept(); }

QString ExplorerLocal::getCurrentPath()
{
    const auto* fileModel{dynamic_cast<QFileSystemModel*>(model())};

    QString path{fileModel->fileInfo(rootIndex()).canonicalFilePath()};

    return path;
}

bool ExplorerLocal::fileIsValid(QString file)
{
    QFileInfo fileInfo(file);

    if (isOpen())
        return (QFile::exists(file) && fileInfo.isFile() &&
                fileInfo.isReadable());

    if (QFile::exists(file))
        return (fileInfo.isFile() && fileInfo.isWritable());

    bool dirExists{fileInfo.dir().exists()};
    bool writable{QFileInfo(fileInfo.dir().canonicalPath()).isWritable()};
    return (dirExists && writable);
}

bool ExplorerLocal::isWrappingContent() { return QListView::isWrapping(); }
