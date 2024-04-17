#include <QListView>
#include <QScroller>
#include <QDebug>
#include <QTimer>

#include "Explorer.h"
#include "Common.h"

Explorer::Explorer(bool open) :
    open_(open)
{

}

Explorer::~Explorer()
{

}

void Explorer::setupList(QListView* listView)
{
#ifdef Q_OS_ANDROID
    QScroller* scroller = QScroller::scroller(listView->viewport());

    scroller->grabGesture(listView->viewport(), QScroller::LeftMouseButtonGesture);
#endif

    QObject::connect(listView,
                     SIGNAL(doubleClicked(QModelIndex)),
                     listView,
                     SLOT(itemActivated(QModelIndex)));

    listView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listView->setDropIndicatorShown(false);
    listView->setSelectionBehavior(QAbstractItemView::SelectItems);
    listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    listView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    listView->setResizeMode(QListView::Adjust);
    listView->setDragDropMode(QAbstractItemView::NoDragDrop);
}
