#include "CursorPointerSelector.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPen>

CursorPointerSelector::CursorPointerSelector(CursorDirection direction,
                                             QWidget* parent)
    : CursorPointerTextEdit(parent), diretion_{direction}
{
    resize(size_, size_ + size_ / 4);
}

void CursorPointerSelector::paintEvent([[maybe_unused]] QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QColor color(Qt::black);
    color.setAlpha(150);
    QBrush brush(color);
    QPainterPath path;
    int roundSize = size_ / 6;
    if (CURSOR_DIRECTION_RIGHT == diretion_)
    {
        path.moveTo(0, 0);
        path.lineTo(size_, size_ / 4);
        path.lineTo(size_, size_ + size_ / 4 - roundSize);
        path.lineTo(size_ - roundSize, size_ + size_ / 4);
        path.lineTo(0, size_ + size_ / 4);
    }
    else
    {
        path.moveTo(0, size_ / 4);
        path.lineTo(size_, 0);
        path.lineTo(size_, size_ + size_ / 4);
        path.lineTo(roundSize, size_ + size_ / 4);
        path.lineTo(0, size_ + size_ / 4 - roundSize);
    }
    path.closeSubpath();
    painter.fillPath(path, brush);
}

QPoint CursorPointerSelector::calculateOffset(QMouseEvent* event)
{
    return QPoint(size_ / 2, event->pos().y());
}

void CursorPointerSelector::moveVisualPointer(int x, int y)
{
    if (CURSOR_DIRECTION_RIGHT == diretion_)
        move(x, y);
    else
        move(x - size_, y);
}

QPoint CursorPointerSelector::calculateNewPosition(QPoint movePoint)
{
    if (CURSOR_DIRECTION_RIGHT == diretion_)
        return QPoint(movePoint.x(), movePoint.y());

    return QPoint(movePoint.x() + size_, movePoint.y());
}
