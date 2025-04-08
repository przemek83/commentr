#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPen>

#include "Config.h"
#include "CursorPointer.h"

CursorPointer::CursorPointer(QWidget* parent) : QWidget(parent), dragged_(false)
{
    setPalette(Qt::transparent);

    hide();

    updateSize();
}

void CursorPointer::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::blue));
    QBrush brush(QColor(0, 0, 255, 150));
    QPainterPath path;
    int roundSize = size_ / 6;
    path.moveTo(0, size_ / 4);
    path.lineTo(size_ / 2, 0);
    path.lineTo(size_, size_ / 4);
    path.lineTo(size_, size_ + size_ / 4 - roundSize);
    path.lineTo(size_ - roundSize, size_ + size_ / 4);
    path.lineTo(roundSize, size_ + size_ / 4);
    path.lineTo(0, size_ + size_ / 4 - roundSize);
    path.closeSubpath();
    painter.fillPath(path, brush);
}

void CursorPointer::mousePressEvent(QMouseEvent* event)
{
    offset_ = calculateOffset(event);
    dragged_ = true;
    positionChanged(event);
}

QPoint CursorPointer::calculateOffset(QMouseEvent* event)
{
    return QPoint(size_ / 2, event->pos().y());
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
    size_ = Config::getInstance().uiSize();
    resize(size_, size_ + size_ / 4);
}

bool CursorPointer::dragged() const { return dragged_; }
