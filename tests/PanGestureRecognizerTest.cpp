#include "PanGestureRecognizerTest.h"

#include "PanGestureRecognizer.h"

#include <QPanGesture>
#include <QTest>
#include <QTouchEvent>
#include <memory>

namespace
{
QTouchEvent createTouchEvent(const QList<QEventPoint>& points,
                             QEvent::Type type)
{
    return QTouchEvent(type, nullptr, Qt::NoModifier, points);
}

QEventPoint createTouchPoint(QPointF point)
{
    QEventPoint eventPoint{0, QEventPoint::Pressed, point, point};
    return eventPoint;
}

std::unique_ptr<QPanGesture> createGesture(PanGestureRecognizer& recognizer)
{
    QGestureRecognizer* baseRecognizer{&recognizer};
    return std::unique_ptr<QPanGesture>(
        static_cast<QPanGesture*>(baseRecognizer->create(nullptr)));
}

}  // namespace

void PanGestureRecognizerTest::recognizeTouchBeginSingleTouchMayBeGesture()
{
    PanGestureRecognizer recognizer;
    const std::unique_ptr<QPanGesture> gesture{createGesture(recognizer)};

    auto beginEvent{
        createTouchEvent({createTouchPoint({1.0, 1.0})}, QEvent::TouchBegin)};

    const auto result{
        recognizer.recognize(gesture.get(), nullptr, &beginEvent)};

    QCOMPARE(result, QGestureRecognizer::MayBeGesture);
    QCOMPARE(gesture->offset(), QPointF());
    QCOMPARE(gesture->lastOffset(), QPointF());
}

void PanGestureRecognizerTest::recognizeTouchBeginMultiTouchIgnored()
{
    PanGestureRecognizer recognizer;
    const std::unique_ptr<QPanGesture> gesture{createGesture(recognizer)};

    const auto points{QList<QEventPoint>{createTouchPoint({1.0, 1.0}),
                                         createTouchPoint({2.0, 2.0})}};
    auto beginEvent{createTouchEvent(points, QEvent::TouchBegin)};

    const auto result{
        recognizer.recognize(gesture.get(), nullptr, &beginEvent)};

    QCOMPARE(result, QGestureRecognizer::Ignore);
}

void PanGestureRecognizerTest::
    recognizeTouchUpdateAboveThresholdTriggersGesture()
{
    PanGestureRecognizer recognizer;
    const std::unique_ptr<QPanGesture> gesture{createGesture(recognizer)};

    auto beginEvent{
        createTouchEvent({createTouchPoint({0.0, 0.0})}, QEvent::TouchBegin)};
    recognizer.recognize(gesture.get(), nullptr, &beginEvent);

    auto updateEvent{
        createTouchEvent({createTouchPoint({15.0, 0.0})}, QEvent::TouchUpdate)};

    const auto result{
        recognizer.recognize(gesture.get(), nullptr, &updateEvent)};

    QCOMPARE(result, QGestureRecognizer::TriggerGesture);
}

void PanGestureRecognizerTest::recognizeTouchUpdateBelowThresholdMayBeGesture()
{
    PanGestureRecognizer recognizer;
    const std::unique_ptr<QPanGesture> gesture{createGesture(recognizer)};

    auto beginEvent{
        createTouchEvent({createTouchPoint({0.0, 0.0})}, QEvent::TouchBegin)};
    recognizer.recognize(gesture.get(), nullptr, &beginEvent);

    auto updateEvent{
        createTouchEvent({createTouchPoint({9.0, 0.0})}, QEvent::TouchUpdate)};

    const auto result{
        recognizer.recognize(gesture.get(), nullptr, &updateEvent)};

    QCOMPARE(result, QGestureRecognizer::MayBeGesture);
}

void PanGestureRecognizerTest::recognizeTouchEndCompletesGesture()
{
    QSKIP("Skip temporarily.");
    PanGestureRecognizer recognizer;
    const std::unique_ptr<QPanGesture> gesture{createGesture(recognizer)};

    auto beginEvent{
        createTouchEvent({createTouchPoint({0.0, 0.0})}, QEvent::TouchBegin)};
    recognizer.recognize(gesture.get(), nullptr, &beginEvent);

    auto updateEvent{createTouchEvent({createTouchPoint({20.0, 20.0})},
                                      QEvent::TouchUpdate)};
    recognizer.recognize(gesture.get(), nullptr, &updateEvent);

    auto endEvent{
        createTouchEvent({createTouchPoint({30.0, 30.0})}, QEvent::TouchEnd)};

    const auto result{recognizer.recognize(gesture.get(), nullptr, &endEvent)};

    QCOMPARE(result, QGestureRecognizer::FinishGesture);
}

void PanGestureRecognizerTest::resetClearsGestureState()
{
    PanGestureRecognizer recognizer;
    const std::unique_ptr<QPanGesture> gesture{createGesture(recognizer)};

    auto beginEvent{
        createTouchEvent({createTouchPoint({0.0, 0.0})}, QEvent::TouchBegin)};
    recognizer.recognize(gesture.get(), nullptr, &beginEvent);

    recognizer.reset(gesture.get());

    QCOMPARE(gesture->state(), Qt::NoGesture);
}
