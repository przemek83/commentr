#include "Local.h"

#include <QFileSystemModel>
#include <QHeaderView>
#include <QScroller>
#include <QTimer>

Local::Local(bool open, QWidget* parent)
    : QTableView(parent), Explorer(open), timeForSecondClickIsUp_(false)
{
    initModelAndView();

    BrowseFilesSelectionHandler* handler =
        new BrowseFilesSelectionHandler(this);
    connect(handler, SIGNAL(releasePosition(QPointF)), this,
            SLOT(selectItemOnRelease(QPointF)));
    viewport()->installEventFilter(handler);

    QScroller::grabGesture(viewport());

    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setDropIndicatorShown(false);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    // Hide type.
    hideColumn(2);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    verticalHeader()->hide();
    horizontalHeader()->hide();
    setGridStyle(Qt::NoPen);
}

Local::~Local()
{
    QFileSystemModel* fileModel = static_cast<QFileSystemModel*>(model());

    QModelIndex currentIndex = rootIndex();

    QString lastDir = fileModel->fileInfo(currentIndex).absoluteFilePath();
    Config::getInstance().setLastPickedDir(lastDir);
}

void Local::setPath(QString path)
{
    QFileSystemModel* fileModel = static_cast<QFileSystemModel*>(model());

    setRootIndex(
        fileModel->index((path.isEmpty() ? Common::rootPath() : path)));
    QModelIndex newRootIndex = fileModel->index(0, 0, rootIndex());
    setCurrentIndex(newRootIndex);
}

QString Local::getCurrentPath() { return currentIndex().data().toString(); }

void Local::initModelAndView()
{
    // Do not show on listView current item.
    setFocusPolicy(Qt::NoFocus);

    QFileSystemModel* fileModel = new QFileSystemModel(this);

    fileModel->setFilter(QDir::AllEntries | QDir::AllDirs | QDir::NoDot |
                         QDir::System | QDir::Hidden);

    fileModel->setRootPath(Common::rootPath());

    connect(fileModel, SIGNAL(directoryLoaded(QString)), this,
            SLOT(directoryLoaded(QString)));

    setModel(fileModel);

    // Set proper initialization path.
    QString initPath = Config::getInstance().lastPickedDir();
    if (!fileModel->index(initPath).isValid())
    {
        initPath = Common::rootPath();
    }

    setRootIndex(fileModel->index(initPath));
}

void Local::itemDoubleClicked(QModelIndex index)
{
    QFileSystemModel* fileModel = static_cast<QFileSystemModel*>(model());

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

        QString path =
            QString(fileModel->fileInfo(rootIndex()).canonicalFilePath());
        if (!path.endsWith(Common::rootPath()))
            path += Common::rootPath();

        emit pathChanged(path);
    }
    else
    {
        if (fileIsValid(canonicalFilePath))
            emit filePicked(canonicalFilePath);
    }
}

void Local::directoryLoaded(const QString& path)
{
    directoryIsAccessible(path);
    resizeColumnsToContents();
}

bool Local::directoryIsAccessible(const QString& path)
{
    QFileSystemModel* fileModel = static_cast<QFileSystemModel*>(model());

    QDir newDir(path);
    if (newDir.entryList(QDir::AllEntries).count() == 0)
    {
        emit fileOrDirNotAccessible(tr("Not accessible..."));

        if (newDir.cdUp())
        {
            setRootIndex(fileModel->index(newDir.absolutePath()));
        }
        else
        {
            emit fileOrDirNotAccessible(tr("I/O error..."));
            setRootIndex(fileModel->index(""));
        }

        return false;
    }

    return true;
}

void Local::listViewItemClicked(const QModelIndex& index)
{
    QFileSystemModel* fileModel = static_cast<QFileSystemModel*>(model());

    if (!fileModel->isDir(index))
        emit pathChanged(fileModel->filePath(index));
}

void Local::selectItemOnRelease(QPointF pos)
{
    QModelIndex itemToSelect = indexAt(pos.toPoint());
    static const int doubleClickTime{Common::doubleClickTime()};

    QItemSelectionModel* selectModel = selectionModel();
    if (selectModel->isSelected(itemToSelect))
    {
        if (!timeForSecondClickIsUp_)
        {
            itemDoubleClicked(itemToSelect);
        }
        else
        {
            QTimer::singleShot(doubleClickTime, this,
                               SLOT(timeForSecondClickIsUp()));

            timeForSecondClickIsUp_ = false;
        }
    }
    else
    {
        selectModel->select(itemToSelect, QItemSelectionModel::ClearAndSelect);

        listViewItemClicked(itemToSelect);

        QTimer::singleShot(doubleClickTime, this,
                           SLOT(timeForSecondClickIsUp()));

        timeForSecondClickIsUp_ = false;
    }
}

void Local::timeForSecondClickIsUp() { timeForSecondClickIsUp_ = true; }

bool Local::fileIsValid(QString file)
{
    QFileInfo fileInfo(file);

    if (open_)
        return (fileInfo.exists() && fileInfo.isFile() &&
                fileInfo.isReadable());

    if (fileInfo.exists())
        return (fileInfo.isFile() && fileInfo.isWritable());

    bool dirExists{fileInfo.dir().exists()};
    bool writable{QFileInfo(fileInfo.dir().canonicalPath()).isWritable()};
    return (dirExists && writable);
}

bool Local::isWrapping() { return false; }

void Local::setWrapping(bool /*wrapping*/) {}

void Local::selectionChanged(const QItemSelection& selected,
                             const QItemSelection& deselected)
{
    selectRow(selected.first().indexes().first().row());
    QTableView::selectionChanged(selected, deselected);
    resizeRowsToContents();
    selectionModel()->select(
        selected.first().indexes().first(),
        QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    repaint();
}
