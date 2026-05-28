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
    {
        return result;
    }

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
    const QList<QTouchEvent::TouchPoint>& touchPoints{getTouchPoints(event)};

    QGestureRecognizer::Result result{QGestureRecognizer::Ignore};

    if (touchPoints.size() == singleTouchPoint_)
    {
        result = MayBeGesture;
        panGesture->setLastOffset(QPointF());
        panGesture->setOffset(QPointF());
    }

    return result;
}

QGestureRecognizer::Result PanGestureRecognizer::manageTouchUpdate(
    QPanGesture* panGesture, QEvent* event)
{
    const QList<QTouchEvent::TouchPoint>& touchPoints{getTouchPoints(event)};

    QGestureRecognizer::Result result{QGestureRecognizer::Ignore};
    if (touchPoints.size() == singleTouchPoint_)
    {
        panGesture->setLastOffset(panGesture->offset());

        QPointF resultPoint = touchPoints.first().position() -
                              touchPoints.first().pressPosition();
        panGesture->setOffset(resultPoint);

        if ((panGesture->offset().x() > panTriggerDistance_) ||
            (panGesture->offset().y() > panTriggerDistance_) ||
            (panGesture->offset().x() < -panTriggerDistance_) ||
            (panGesture->offset().y() < -panTriggerDistance_))
        {
            panGesture->setHotSpot(touchPoints.first().globalPressPosition());
            result = QGestureRecognizer::TriggerGesture;
        }
        else
        {
            result = QGestureRecognizer::MayBeGesture;
        }
    }

    return result;
}

QGestureRecognizer::Result PanGestureRecognizer::manageTouchEnd(
    QPanGesture* panGesture, QEvent* event)
{
    QGestureRecognizer::Result result{QGestureRecognizer::Ignore};

    if (panGesture->state() != Qt::NoGesture)
    {
        const QList<QTouchEvent::TouchPoint>& touchPoints{
            getTouchPoints(event)};

        if (touchPoints.size() == singleTouchPoint_)
        {
            panGesture->setLastOffset(panGesture->offset());

            QPointF resultPoint = touchPoints.first().position() -
                                  touchPoints.first().pressPosition();
            panGesture->setOffset(resultPoint);
        }

        result = QGestureRecognizer::FinishGesture;
    }
    else
    {
        result = QGestureRecognizer::CancelGesture;
    }

    return result;
}
