#include "FileExplorer.h"

#include <QFileSystemModel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QScrollBar>

#include "../Common.h"
#include "../Config.h"
#include "../File.h"

FileExplorer::FileExplorer(bool open, Config& config, QWidget* parent)
    : QListView(parent), config_(config), open_{open}
{
    horizontalScrollBar()->setStyleSheet(Common::getStyleSheet());
    verticalScrollBar()->setStyleSheet(Common::getStyleSheet());

    setWrapping(!config_.listViewInBrowser());

    setupList();
}

FileExplorer::~FileExplorer()
{
    const auto* fileModel{dynamic_cast<QFileSystemModel*>(model())};
    QModelIndex currentIndex{rootIndex()};
    QString lastDir{fileModel->fileInfo(currentIndex).absoluteFilePath()};
    config_.setLastPickedDir(lastDir);
}

void FileExplorer::setupList()
{
#ifdef Q_OS_ANDROID
    QScroller* scroller = QScroller::scroller(viewport());

    scroller->grabGesture(viewport(), QScroller::LeftMouseButtonGesture);
#endif

    QObject::connect(this, &FileExplorer::clicked, this,
                     &FileExplorer::itemWasActivated);

    QObject::connect(this, &FileExplorer::doubleClicked, this,
                     &FileExplorer::itemWasActivated);

    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setDropIndicatorShown(false);
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setResizeMode(QListView::Adjust);
    setDragDropMode(QAbstractItemView::NoDragDrop);
}

void FileExplorer::changeRootIndex(const QFileSystemModel& model,
                                   const QString& path)
{
    QString pathToUse{path};
    if (pathToUse.isEmpty())
        pathToUse = Common::rootPath();
    setRootIndex(model.index(pathToUse));
}

void FileExplorer::setPath(const QString& path)
{
    const auto* fileModel{dynamic_cast<QFileSystemModel*>(model())};
    changeRootIndex(*fileModel, path);
    QModelIndex newRootIndex{fileModel->index(0, 0, rootIndex())};
    setCurrentIndex(newRootIndex);
}

void FileExplorer::initialize()
{
    auto* fileModel{new QFileSystemModel(this)};
    fileModel->setFilter(QDir::AllEntries | QDir::AllDirs | QDir::NoDot |
                         QDir::System | QDir::Hidden);
    fileModel->setRootPath(Common::rootPath());

    connect(fileModel, &QFileSystemModel::directoryLoaded, this,
            &FileExplorer::directoryLoaded);

    setModel(fileModel);

    changeRootIndex(*fileModel, config_.lastPickedDir());
}

bool FileExplorer::isInitialized() const { return model() != nullptr; }

void FileExplorer::performOperationOnFile(const QString& filePath)
{
    if (!fileIsValid(filePath))
    {
        QMessageBox::information(this, tr("Error"), tr("Cannot open file..."));
        return;
    }

    QFileInfo fileInfo(filePath);
    QString filePathToUse{fileInfo.filePath()};
    QString content;

    if (open_)
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

    Q_EMIT filePrepared(file);
}

void FileExplorer::itemWasActivated(QModelIndex index)
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

        Q_EMIT pathChanged(getCurrentPath());
    }
    else
    {
        performOperationOnFile(canonicalFilePath);
    }
}

void FileExplorer::directoryLoaded(const QString& path)
{
    QString oldCurrentPath = getCurrentPath();

    directoryIsAccessible(path);

    QString newCurrentPath = getCurrentPath();
    if (oldCurrentPath != newCurrentPath)
        Q_EMIT pathChanged(newCurrentPath);
}

bool FileExplorer::directoryIsAccessible(const QString& path)
{
    const auto* fileModel{dynamic_cast<QFileSystemModel*>(model())};

    if (QDir newDir(path); newDir.entryList(QDir::AllEntries).empty())
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

void FileExplorer::mouseMoveEvent(QMouseEvent* e) { e->accept(); }

QString FileExplorer::getCurrentPath() const
{
    const auto* fileModel{dynamic_cast<QFileSystemModel*>(model())};

    QString path{fileModel->fileInfo(rootIndex()).canonicalFilePath()};

    return path;
}

bool FileExplorer::fileIsValid(const QString& file) const
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

bool FileExplorer::isWrappingContent() const { return QListView::isWrapping(); }
