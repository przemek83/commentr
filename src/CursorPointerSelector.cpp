#include "CursorPointerSelector.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPen>

CursorPointerSelector::CursorPointerSelector(CursorDirection direction,
                                             Config& config, QWidget* parent)
    : CursorPointerTextEdit(config, parent), diretion_{direction}
{
    resize(size_, size_ + size_ / pointerBodyDivisor_);
}

void CursorPointerSelector::paintEvent([[maybe_unused]] QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QColor color(Qt::black);
    constexpr int pointerAlpha{150};
    color.setAlpha(pointerAlpha);
    QBrush brush(color);
    QPainterPath path;
    constexpr int pointerRoundDivisor{6};
    const int roundSize = size_ / pointerRoundDivisor;
    if (CursorDirection::RIGHT == diretion_)
    {
        path.moveTo(0, 0);
        path.lineTo(size_, size_ / pointerBodyDivisor_);
        path.lineTo(size_, size_ + size_ / pointerBodyDivisor_ - roundSize);
        path.lineTo(size_ - roundSize, size_ + size_ / pointerBodyDivisor_);
        path.lineTo(0, size_ + size_ / pointerBodyDivisor_);
    }
    else
    {
        path.moveTo(0, size_ / pointerBodyDivisor_);
        path.lineTo(size_, 0);
        path.lineTo(size_, size_ + size_ / pointerBodyDivisor_);
        path.lineTo(roundSize, size_ + size_ / pointerBodyDivisor_);
        path.lineTo(0, size_ + size_ / pointerBodyDivisor_ - roundSize);
    }
    path.closeSubpath();
    painter.fillPath(path, brush);
}

QPoint CursorPointerSelector::calculateOffset(QMouseEvent* event)
{
    constexpr int pointerHalfDivisor{2};
    return QPoint(size_ / pointerHalfDivisor, event->pos().y());
}

void CursorPointerSelector::moveVisualPointer(int x, int y)
{
    if (CursorDirection::RIGHT == diretion_)
        move(x, y);
    else
        move(x - size_, y);
}

QPoint CursorPointerSelector::calculateNewPosition(QPoint movePoint)
{
    if (CursorDirection::RIGHT == diretion_)
        return QPoint(movePoint.x(), movePoint.y());

    return QPoint(movePoint.x() + size_, movePoint.y());
}
