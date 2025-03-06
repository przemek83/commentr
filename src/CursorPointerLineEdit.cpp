#include <QMouseEvent>

#include "CursorPointerLineEdit.h"

CursorPointerLineEdit::CursorPointerLineEdit(int addLeftMargin, QWidget *parent) :
    CursorPointer(parent), margin_(2), scrollingNeeded_(false),
    addLeftMargin_(addLeftMargin)
{

}

CursorPointerLineEdit::~CursorPointerLineEdit()
{

}

QPoint CursorPointerLineEdit::calcMovePoint(QPoint mousePos)
{
    QPoint movePoint = mapToParent(mousePos - offset_);

    if( 0 == range_.height() )
    {
        movePoint.setY(range_.y());
    }

    if( movePoint.x() > range_.width() - size_/2 + range_.x())
    {
        movePoint.setX(range_.width() - size_/2 + range_.x());
    }

    //Margins added to able to scroll.
    if( movePoint.x() < range_.x() - size_/2 + (scrollingNeeded_ ? 0 : addLeftMargin_))
    {
        movePoint.setX(range_.x() - size_/2 + (scrollingNeeded_ ? 0 : addLeftMargin_));
    }

    return movePoint;
}


void CursorPointerLineEdit::positionChanged(QMouseEvent* event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        QPoint movePoint = calcMovePoint(event->pos());

        move(movePoint.x(), movePoint.y());
        emit pointerMoved(QPoint(movePoint.x() + size_/2 - range_.x() - margin_,
                                 movePoint.y()));
    }
}

void CursorPointerLineEdit::moveVisualPointer(int x, int y)
{
    move(x - size_/2 - margin_, y);
}

void CursorPointerLineEdit::setScrollingNeeded(bool scrollingNeeded)
{
    scrollingNeeded_ = scrollingNeeded;
}
