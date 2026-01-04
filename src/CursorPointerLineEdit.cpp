#include "CursorPointerLineEdit.h"

#include <QMouseEvent>

CursorPointerLineEdit::CursorPointerLineEdit(int addLeftMargin, QWidget* parent)
    : CursorPointer(parent),
      margin_(2),
      scrollingNeeded_(false),
      addLeftMargin_(addLeftMargin)
{
}

QPoint CursorPointerLineEdit::calcMovePoint(QPoint mousePos)
{
    QPoint movePoint = mapToParent(mousePos - offset_);

    if (range_.height() == 0)
        movePoint.setY(range_.y());

    int baseLeftBoundary{range_.x() - size_ / 2};
    if (movePoint.x() > range_.width() + baseLeftBoundary)
        movePoint.setX(range_.width() + baseLeftBoundary);

    // Margins added to able to scroll.
    int minCursorX{baseLeftBoundary};
    if (!scrollingNeeded_)
        minCursorX += addLeftMargin_;

    if (movePoint.x() < minCursorX)
        movePoint.setX(minCursorX);

    return movePoint;
}

void CursorPointerLineEdit::positionChanged(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        const QPoint movePoint{calcMovePoint(event->pos())};

        move(movePoint.x(), movePoint.y());
        emit pointerMoved(QPoint(
            movePoint.x() + size_ / 2 - range_.x() - margin_, movePoint.y()));
    }
}

void CursorPointerLineEdit::moveVisualPointer(int x, int y)
{
    move(x - size_ / 2 - margin_, y);
}

void CursorPointerLineEdit::setScrollingNeeded(bool scrollingNeeded)
{
    scrollingNeeded_ = scrollingNeeded;
}
