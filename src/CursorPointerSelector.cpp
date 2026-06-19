#include "CursorPointerSelector.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPen>

CursorPointerSelector::CursorPointerSelector(CursorDirection direction,
                                             Config& config, QWidget* parent)
    : CursorPointerTextEdit(config, parent), diretion_{direction}
{
    resize(getSize(), getSize() + getQuarterOfSize());
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
    const int size{getSize()};
    const int roundSize = size / pointerRoundDivisor;
    if (CursorDirection::RIGHT == diretion_)
    {
        path.moveTo(0, 0);
        path.lineTo(size, getQuarterOfSize());
        path.lineTo(size, (size + getQuarterOfSize()) - roundSize);
        path.lineTo(size - roundSize, size + getQuarterOfSize());
        path.lineTo(0, size + getQuarterOfSize());
    }
    else
    {
        path.moveTo(0, getQuarterOfSize());
        path.lineTo(size, 0);
        path.lineTo(size, size + getQuarterOfSize());
        path.lineTo(roundSize, size + getQuarterOfSize());
        path.lineTo(0, (size + getQuarterOfSize()) - roundSize);
    }
    path.closeSubpath();
    painter.fillPath(path, brush);
}

QPoint CursorPointerSelector::calculateOffset(QMouseEvent* event)
{
    return {getHalfOfSize(), event->pos().y()};
}

void CursorPointerSelector::moveVisualPointer(int x, int y)
{
    if (CursorDirection::RIGHT == diretion_)
        move(x, y);
    else
        move(x - getSize(), y);
}

QPoint CursorPointerSelector::calculateNewPosition(QPoint movePoint)
{
    if (CursorDirection::RIGHT == diretion_)
        return {movePoint.x(), movePoint.y()};

    return {movePoint.x() + getSize(), movePoint.y()};
}

int CursorPointerSelector::getQuarterOfSize() const
{
    return getSize() / pointerBodyDivisor_;
}
