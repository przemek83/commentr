#include "CursorPointer.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPen>

#include "Config.h"

CursorPointer::CursorPointer(Config& config, QWidget* parent)
    : QWidget(parent), dragged_(false), config_(config)
{
    setPalette(Qt::transparent);

    hide();

    updateSize();
}

void CursorPointer::paintEvent([[maybe_unused]] QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::blue));

    constexpr int pointerColorBlue{255};
    constexpr int pointerColorAlpha{150};
    QBrush brush(QColor(0, 0, pointerColorBlue, pointerColorAlpha));
    QPainterPath path{createPath()};
    painter.fillPath(path, brush);
}

QPainterPath CursorPointer::createPath() const
{
    QPainterPath path;
    constexpr int roundCornerDivisor{6};
    const int roundSize = size_ / roundCornerDivisor;
    const auto size{static_cast<double>(size_)};
    const float quarterOfSize{static_cast<float>(size / pointerTipDivisor_)};
    path.moveTo(0, quarterOfSize);
    path.lineTo(size / pointerHalfDivisor_, 0);
    path.lineTo(size, quarterOfSize);
    path.lineTo(size, size + quarterOfSize - roundSize);
    path.lineTo(size - roundSize, size + quarterOfSize);
    path.lineTo(roundSize, size + quarterOfSize);
    path.lineTo(0, size + quarterOfSize - roundSize);
    path.closeSubpath();
    return path;
}

void CursorPointer::mousePressEvent(QMouseEvent* event)
{
    offset_ = calculateOffset(event);
    dragged_ = true;
    positionChanged(event);
}

QPoint CursorPointer::calculateOffset(QMouseEvent* event)
{
    return QPoint(size_ / pointerHalfDivisor_, event->pos().y());
}

void CursorPointer::mouseMoveEvent(QMouseEvent* event)
{
    positionChanged(event);
}

void CursorPointer::showCursor()
{
    dragged_ = true;
    show();
}

void CursorPointer::mouseReleaseEvent(QMouseEvent* event)
{
    dragged_ = false;

    emit mouseReleased();

    QWidget::mouseReleaseEvent(event);
}

void CursorPointer::setRange(QRect range) { range_ = range; }

void CursorPointer::changeEvent(QEvent* event)
{
    if (QEvent::StyleChange == event->type())
        updateSize();

    QWidget::changeEvent(event);
}

void CursorPointer::updateSize()
{
    size_ = static_cast<int>(config_.uiSize());
    resize(size_, size_ + size_ / pointerTipDivisor_);
}

bool CursorPointer::dragged() const { return dragged_; }
