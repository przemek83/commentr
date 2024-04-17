#ifndef PANGESTURERECOGNIZER_H
#define PANGESTURERECOGNIZER_H

#include <QGestureRecognizer>
#include <QTouchEvent>

class QGesture;

class PanGestureRecognizer : public QGestureRecognizer
{
public:
    PanGestureRecognizer();

private:
    virtual QGesture* create(QObject* target) override;
    virtual Result recognize(QGesture* state, QObject* watched, QEvent* event) override;
    const QList<QTouchEvent::TouchPoint>& getTouchPoints(QEvent* event);
};

#endif // PANGESTURERECOGNIZER_H
