#include "Explorer.h"

#include <QListView>
#include <QScroller>
#include <QTimer>

#include "../Common.h"

Explorer::Explorer(bool open) : open_(open) {}

void Explorer::setupList(QListView* listView)
{
#ifdef Q_OS_ANDROID
    QScroller* scroller = QScroller::scroller(listView->viewport());

    scroller->grabGesture(listView->viewport(),
                          QScroller::LeftMouseButtonGesture);
#endif

    QObject::connect(listView, SIGNAL(clicked(QModelIndex)), listView,
                     SLOT(itemWasActivated(QModelIndex)));

    QObject::connect(listView, SIGNAL(doubleClicked(QModelIndex)), listView,
                     SLOT(itemWasActivated(QModelIndex)));

    listView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listView->setDropIndicatorShown(false);
    listView->setSelectionBehavior(QAbstractItemView::SelectItems);
    listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    listView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    listView->setResizeMode(QListView::Adjust);
    listView->setDragDropMode(QAbstractItemView::NoDragDrop);
}
