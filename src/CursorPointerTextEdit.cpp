#include "CursorPointerTextEdit.h"

#include <QMouseEvent>
#include <QTimer>

#include "Common.h"

CursorPointerTextEdit::CursorPointerTextEdit(Config& config, QWidget* parent)
    : CursorPointer(config, parent)
{
}

void CursorPointerTextEdit::moveVisualPointer(int x, int y)
{
    move(x - (size_ / pointerHalfDivisor_), y);
}

QPoint CursorPointerTextEdit::calcMovePoint(QPoint mousePos)
{
    QPoint movePoint{mapToParent(mousePos - getOffset())};

    return movePoint;
}

void CursorPointerTextEdit::positionChanged(QMouseEvent* event)
{
    if (event->buttons().testFlag(Qt::LeftButton))
    {
        const QPoint movePoint{calcMovePoint(event->pos())};

        move(movePoint.x(), movePoint.y());
        if (canEmitPointerMoved_)
        {
            Q_EMIT pointerMoved(calculateNewPosition(movePoint));
            static const int timerFireInterval = Common::timerFireInterval();
            QTimer::singleShot(timerFireInterval, this,
                               [&canEmit = canEmitPointerMoved_]()
                               { canEmit = true; });
        }

        canEmitPointerMoved_ = false;
    }
}

QPoint CursorPointerTextEdit::calculateNewPosition(QPoint movePoint)
{
    return {movePoint.x() + (size_ / pointerHalfDivisor_), movePoint.y()};
}
