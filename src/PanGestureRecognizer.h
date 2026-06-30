#ifndef PANGESTURERECOGNIZER_H
#define PANGESTURERECOGNIZER_H

#include <QGestureRecognizer>
#include <QTouchEvent>

class QGesture;
class QPanGesture;

class PanGestureRecognizer : public QGestureRecognizer
{
public:
    PanGestureRecognizer() = default;
    ~PanGestureRecognizer() override = default;

private:
    Q_DISABLE_COPY_MOVE(PanGestureRecognizer)

    QGesture* create(QObject* target) override;
    Result recognize(QGesture* state, QObject* watched, QEvent* event) override;
    static const QList<QTouchEvent::TouchPoint>& getTouchPoints(QEvent* event);

    static QGestureRecognizer::Result manageTouchBegin(QPanGesture* panGesture,
                                                       QEvent* event);
    static QGestureRecognizer::Result manageTouchUpdate(QPanGesture* panGesture,
                                                        QEvent* event);
    static QGestureRecognizer::Result manageTouchEnd(QPanGesture* panGesture,
                                                     QEvent* event);

    static bool isPanThresholdExceeded(QPointF offset);

    static void rotateOffset(QPanGesture* panGesture,
                             const QEventPoint& eventPoint);

    static bool isMultiTouchEvent(QEvent* event);

    static constexpr int singleTouchPoint_{1};
    static constexpr int panTriggerDistance_{10};
};

#endif  // PANGESTURERECOGNIZER_H
