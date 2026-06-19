#include "CursorPointerLineEdit.h"

#include <QMouseEvent>

CursorPointerLineEdit::CursorPointerLineEdit(int addLeftMargin, Config& config,
                                             QWidget* parent)
    : CursorPointer(config, parent), addLeftMargin_(addLeftMargin)
{
}

QPoint CursorPointerLineEdit::calcMovePoint(QPoint mousePos)
{
    QPoint movePoint = mapToParent(mousePos - getOffset());

    const QRect range{getRange()};
    if (range.height() == 0)
        movePoint.setY(range.y());

    int baseLeftBoundary{range.x() - getHalfOfSize()};
    if (movePoint.x() > (range.width() + baseLeftBoundary))
        movePoint.setX(range.width() + baseLeftBoundary);

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
    if (event->buttons().testFlag(Qt::LeftButton))
    {
        const QPoint movePoint{calcMovePoint(event->pos())};

        move(movePoint.x(), movePoint.y());

        const int pointerCenterOffset{getHalfOfSize()};
        const int textXOrigin{getRange().x() - pointerMargin_};
        const QPoint textPosition{
            movePoint.x() + (pointerCenterOffset - textXOrigin), movePoint.y()};

        Q_EMIT pointerMoved(textPosition);
    }
}

void CursorPointerLineEdit::moveVisualPointer(int x, int y)
{
    move(x - getHalfOfSize() - pointerMargin_, y);
}

void CursorPointerLineEdit::setScrollingNeeded(bool scrollingNeeded)
{
    scrollingNeeded_ = scrollingNeeded;
}
