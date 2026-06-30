#include "PanGestureRecognizer.h"

#include <QPanGesture>

QGesture* PanGestureRecognizer::create(QObject* target)
{
    return new QPanGesture(target);
}

const QList<QTouchEvent::TouchPoint>& PanGestureRecognizer::getTouchPoints(
    QEvent* event)
{
    const auto* touchEvent{dynamic_cast<QTouchEvent*>(event)};
    return touchEvent->points();
}

QGestureRecognizer::Result PanGestureRecognizer::recognize(
    QGesture* state, [[maybe_unused]] QObject* watched, QEvent* event)
{
    QGestureRecognizer::Result result{QGestureRecognizer::Ignore};

    auto* panGesture{dynamic_cast<QPanGesture*>(state)};
    if (panGesture == nullptr)
        return result;

    switch (event->type())
    {
        case QEvent::TouchBegin:
        {
            result = manageTouchBegin(panGesture, event);
            break;
        }

        case QEvent::TouchUpdate:
        {
            result = manageTouchUpdate(panGesture, event);
            break;
        }

        case QEvent::TouchEnd:
        {
            result = manageTouchEnd(panGesture, event);
            break;
        }

        default:
        {
            break;
        }
    }

    return result;
}

QGestureRecognizer::Result PanGestureRecognizer::manageTouchBegin(
    QPanGesture* panGesture, QEvent* event)
{
    if (isMultiTouchEvent(event))
        return QGestureRecognizer::Ignore;

    panGesture->setLastOffset(QPointF());
    panGesture->setOffset(QPointF());

    return MayBeGesture;
}

QGestureRecognizer::Result PanGestureRecognizer::manageTouchUpdate(
    QPanGesture* panGesture, QEvent* event)
{
    if (isMultiTouchEvent(event))
        return QGestureRecognizer::Ignore;

    const QEventPoint touchPoint{getTouchPoints(event).first()};
    rotateOffset(panGesture, touchPoint);

    QGestureRecognizer::Result result{QGestureRecognizer::Ignore};
    if (isPanThresholdExceeded(panGesture->offset()))
    {
        panGesture->setHotSpot(touchPoint.globalPressPosition());
        result = QGestureRecognizer::TriggerGesture;
    }
    else
    {
        result = QGestureRecognizer::MayBeGesture;
    }

    return result;
}

QGestureRecognizer::Result PanGestureRecognizer::manageTouchEnd(
    QPanGesture* panGesture, QEvent* event)
{
    if (panGesture->state() == Qt::NoGesture)
        return QGestureRecognizer::CancelGesture;

    if (!isMultiTouchEvent(event))
        rotateOffset(panGesture, getTouchPoints(event).first());

    return QGestureRecognizer::FinishGesture;
}

bool PanGestureRecognizer::isPanThresholdExceeded(QPointF offset)
{
    return std::abs(offset.x()) > panTriggerDistance_ ||
           std::abs(offset.y()) > panTriggerDistance_;
}

void PanGestureRecognizer::rotateOffset(QPanGesture* panGesture,
                                        const QEventPoint& eventPoint)
{
    panGesture->setLastOffset(panGesture->offset());
    const QPointF resultPoint{eventPoint.position() -
                              eventPoint.pressPosition()};
    panGesture->setOffset(resultPoint);
}

bool PanGestureRecognizer::isMultiTouchEvent(QEvent* event)
{
    const QList<QTouchEvent::TouchPoint>& points{getTouchPoints(event)};
    return points.size() != singleTouchPoint_;
}
