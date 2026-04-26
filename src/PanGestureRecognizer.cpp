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
    constexpr int singleTouchPoint{1};
    constexpr int panTriggerDistance{10};

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
            const QList<QTouchEvent::TouchPoint>& touchPoints =
                getTouchPoints(event);

            if (touchPoints.size() == singleTouchPoint)
            {
                result = MayBeGesture;
                panGesture->setLastOffset(QPointF());
                panGesture->setOffset(QPointF());
            }

            break;
        }

        case QEvent::TouchUpdate:
        {
            const QList<QTouchEvent::TouchPoint>& touchPoints =
                getTouchPoints(event);

            if (touchPoints.size() == singleTouchPoint)
            {
                panGesture->setLastOffset(panGesture->offset());

                QPointF resultPoint = touchPoints.first().position() -
                                      touchPoints.first().pressPosition();
                panGesture->setOffset(resultPoint);

                if ((panGesture->offset().x() > panTriggerDistance) ||
                    (panGesture->offset().y() > panTriggerDistance) ||
                    (panGesture->offset().x() < -panTriggerDistance) ||
                    (panGesture->offset().y() < -panTriggerDistance))
                {
                    panGesture->setHotSpot(
                        touchPoints.first().globalPressPosition());
                    result = QGestureRecognizer::TriggerGesture;
                }
                else
                {
                    result = QGestureRecognizer::MayBeGesture;
                }
            }

            break;
        }

        case QEvent::TouchEnd:
        {
            if (panGesture->state() != Qt::NoGesture)
            {
                const QList<QTouchEvent::TouchPoint>& touchPoints =
                    getTouchPoints(event);

                if (touchPoints.size() == singleTouchPoint)
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

            break;
        }

        case QEvent::TouchCancel:
        {
            break;
        }

        default:
        {
            break;
        }
    }

    return result;
}
