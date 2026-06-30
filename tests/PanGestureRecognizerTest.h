#ifndef PANGESTURERECOGNIZERTEST_H
#define PANGESTURERECOGNIZERTEST_H

#include <QObject>

class PanGestureRecognizerTest : public QObject
{
    Q_OBJECT

private slots:
    static void recognizeTouchBeginSingleTouchMayBeGesture();
    static void recognizeTouchBeginMultiTouchIgnored();
    static void recognizeTouchUpdateAboveThresholdTriggersGesture();
    static void recognizeTouchUpdateBelowThresholdMayBeGesture();
    static void recognizeTouchEndCompletesGesture();
    static void resetClearsGestureState();
};

#endif  // PANGESTURERECOGNIZERTEST_H
