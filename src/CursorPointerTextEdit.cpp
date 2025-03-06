#include <QMouseEvent>
#include <QTimer>

#include "CursorPointerTextEdit.h"
#include "Common.h"

CursorPointerTextEdit::CursorPointerTextEdit(QWidget *parent) :
    CursorPointer(parent),
    canEmitPointerMoved_(true)
{

}

CursorPointerTextEdit::~CursorPointerTextEdit()
{

}

void CursorPointerTextEdit::moveVisualPointer(int x, int y)
{
    move(x - size_/2, y);
}

QPoint CursorPointerTextEdit::calcMovePoint(QPoint mousePos)
{
    QPoint movePoint = mapToParent(mousePos - offset_);

    return movePoint;
}

void CursorPointerTextEdit::positionChanged(QMouseEvent* event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        QPoint movePoint = calcMovePoint(event->pos());

        move(movePoint.x(), movePoint.y());
        if( true == canEmitPointerMoved_ )
        {
            emit pointerMoved(calculateNewPosition(movePoint));
            static const int timerFireInterval = Common::timerFireInterval();
            QTimer::singleShot(timerFireInterval,
                               this,
                               SLOT(allowEmitPointerMoved()));
        }

        canEmitPointerMoved_ = false;
    }
}

QPoint CursorPointerTextEdit::calculateNewPosition(QPoint movePoint)
{
    return QPoint(movePoint.x() + size_/2, movePoint.y());
}

void CursorPointerTextEdit::allowEmitPointerMoved()
{
    canEmitPointerMoved_ = true;
}
