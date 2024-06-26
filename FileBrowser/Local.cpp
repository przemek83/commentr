#include <QFileSystemModel>
#include <QScroller>
#include <QTimer>
#include <QHeaderView>
#include <QDebug>

#include "Local.h"
#include "Config.h"
#include "BrowseFilesSelectionHandler.h"
#include "Common.h"

Local::Local(bool open, QWidget* parent) :
    QTableView(parent), Explorer(open),
    timeForSecondClickIsUp_(false)
{
    initModelAndView();

    BrowseFilesSelectionHandler* handler = new BrowseFilesSelectionHandler(this);
    connect(handler,
            SIGNAL(releasePosition(QPointF)),
            this,
            SLOT(selectItemOnRelease(QPointF)));
    viewport()->installEventFilter(handler);

    QScroller::grabGesture(viewport());

    //setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setDropIndicatorShown(false);
    //setSelectionBehavior(QAbstractItemView::SelectItems);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    //setResizeMode(QListView::Adjust);

    //setFrameStyle(QFrame::NoFrame);

    //Hide type.
    hideColumn(2);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    verticalHeader()->hide();
    horizontalHeader()->hide();
    setGridStyle(Qt::NoPen);
//    selectionModel();
}

Local::~Local()
{
    QFileSystemModel* fileModel =
        static_cast<QFileSystemModel*>(model());

    QModelIndex currentIndex = rootIndex();

    QString lastDir = fileModel->fileInfo(currentIndex).absoluteFilePath();
    Config::getInstance().setLastPickedDir(lastDir);
}

void Local::setPath(QString path)
{
    QFileSystemModel* fileModel =
        static_cast<QFileSystemModel*>(model());

    setRootIndex(fileModel->index((path.isEmpty() ? Common::rootPath() : path)));
    QModelIndex newRootIndex = fileModel->index(0, 0, rootIndex());
    setCurrentIndex(newRootIndex);

//    QFileSystemModel* model =
//        static_cast<QFileSystemModel*>(model());
//    setCurrentIndex(model->index(rootPath_));
    //    filePathLineEdit_->setText(rootPath_);
}

QString Local::getCurrentPath()
{
    return currentIndex().data().toString();
}

void Local::initModelAndView()
{
    //Do not show on listView current item.
    setFocusPolicy(Qt::NoFocus);

    //Set proper initial icon.
    //setWrapping(!Config::getInstance().listViewInBrowser());

    QFileSystemModel* fileModel = new QFileSystemModel(this);

    fileModel->setFilter(QDir::AllEntries |
                     QDir::AllDirs |
                     QDir::NoDot |
                     QDir::System |
                     QDir::Hidden);

    fileModel->setRootPath(Common::rootPath());

    connect(fileModel,
            SIGNAL(directoryLoaded(QString)),
            this,
            SLOT(directoryLoaded(QString)));

    setModel(fileModel);

    //Set proper initialization path.
    QString initPath = Config::getInstance().lastPickedDir();
    if( false == fileModel->index(initPath).isValid() )
    {
        initPath = Common::rootPath();
    }

    setRootIndex(fileModel->index(initPath));

    //Set line edit text, do not allow emitting signals
    //(virtual methods can not be called).
//    filePathLineEdit_->blockSignals(true);
//    filePathLineEdit_->setText(initPath);
//    filePathLineEdit_->blockSignals(false);
}

void Local::itemDoubleClicked(QModelIndex index)
{
    QFileSystemModel* fileModel =
        static_cast<QFileSystemModel*>(model());

    QString canonicalFilePath(fileModel->fileInfo(index).canonicalFilePath());
    if( fileModel->isDir(index) )
    {
        if( index.data().toString() == ".." )
        {
            setRootIndex(fileModel->index(canonicalFilePath));
            QModelIndex newRootIndex =
                fileModel->index(0, 0, rootIndex());
            setCurrentIndex(newRootIndex);
        }
        else
        {
            if( true == directoryIsAccessible(canonicalFilePath) )
            {
                setRootIndex(index);
            }
        }

        QString path =
            QString(fileModel->fileInfo(rootIndex()).canonicalFilePath());
        if( false == path.endsWith(Common::rootPath()) )
        {
            path += Common::rootPath();
        }
        emit pathChanged(path);
        //filePathLineEdit_->setText(path);
    }
    else
    {
        if( true == fileIsValid(canonicalFilePath) )
        {
            emit filePicked(canonicalFilePath);
        }
    }
}

void Local::directoryLoaded(const QString& path)
{
    directoryIsAccessible(path);
    resizeColumnsToContents();
}

bool Local::directoryIsAccessible(const QString &path)
{
    QFileSystemModel* fileModel =
        static_cast<QFileSystemModel*>(model());

    QDir newDir(path);
    if( 0 == newDir.entryList(QDir::AllEntries).count() )
    {
        emit fileOrDirNotAccessible(tr("Not accessible..."));

        if( true == newDir.cdUp() )
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

void Local::listViewItemClicked(const QModelIndex &index)
{
    QFileSystemModel* fileModel =
        static_cast<QFileSystemModel*>(model());

    if( false == fileModel->isDir(index) )
    {
        emit pathChanged(fileModel->filePath(index));
        //filePathLineEdit_->setText(fileModel->filePath(index));
    }

    //Needed??
    //filePathLineEdit_->clearFocus();
}

void Local::selectItemOnRelease(QPointF pos)
{
    QModelIndex itemToSelect = indexAt(pos.toPoint());
    static const int doubleClickTime = Common::doubleClickTime();

    QItemSelectionModel* selectModel = selectionModel();
    if( true == selectModel->isSelected(itemToSelect) )
    {
        if( false == timeForSecondClickIsUp_ )
        {
            itemDoubleClicked(itemToSelect);
        }
        else
        {
            QTimer::singleShot(doubleClickTime,
                               this,
                               SLOT(timeForSecondClickIsUp()));

            timeForSecondClickIsUp_ = false;
        }
    }
    else
    {
        selectModel->select(itemToSelect,
                               QItemSelectionModel::ClearAndSelect);

        listViewItemClicked(itemToSelect);

        QTimer::singleShot(doubleClickTime,
                           this,
                           SLOT(timeForSecondClickIsUp()));

        timeForSecondClickIsUp_ = false;
    }
}

void Local::timeForSecondClickIsUp()
{
    timeForSecondClickIsUp_ = true;
}

bool Local::fileIsValid(QString file)
{
    QFileInfo fileInfo(file);

    if( true == open_ )
    {
        return (true == fileInfo.exists() &&
                true == fileInfo.isFile() &&
                true == fileInfo.isReadable());
    }
    else
    {
        if( true == fileInfo.exists() )
        {
            return (true == fileInfo.isFile() &&
                    true == fileInfo.isWritable());
        }
        else
        {
            bool dirExists = fileInfo.dir().exists();
            bool writable = QFileInfo(fileInfo.dir().canonicalPath()).isWritable();
            return (dirExists && writable);
        }
    }
}

bool Local::isWrapping()
{
    return false;
}

void Local::setWrapping(bool /*wrapping*/)
{

}

//void Local::currentChanged(const QModelIndex &current, const QModelIndex &previous)
//{
//    qDebug() << __FUNCTION__ << current << previous;
//    //selectRow(current.row());
//}

void Local::selectionChanged(const QItemSelection &selected,
                             const QItemSelection &deselected)
{
    qDebug() << __FUNCTION__ << selected << deselected;
    selectRow(selected.first().indexes().first().row());
    QTableView::selectionChanged(selected, deselected);
    resizeRowsToContents();
    selectionModel()->select(selected.first().indexes().first(),
                             QItemSelectionModel::ClearAndSelect |
                             QItemSelectionModel::Rows);
    repaint();
}
