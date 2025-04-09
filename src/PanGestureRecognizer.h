#ifndef PANGESTURERECOGNIZER_H
#define PANGESTURERECOGNIZER_H

#include <QGestureRecognizer>
#include <QTouchEvent>

class QGesture;

class PanGestureRecognizer : public QGestureRecognizer
{
private:
    QGesture* create(QObject* target) override;
    Result recognize(QGesture* state, QObject* watched, QEvent* event) override;
    static const QList<QTouchEvent::TouchPoint>& getTouchPoints(QEvent* event);
};

#endif  // PANGESTURERECOGNIZER_H
