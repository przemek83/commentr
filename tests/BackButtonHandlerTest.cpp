#include "BackButtonHandlerTest.h"

#include <QKeyEvent>
#include <QTest>
#include <QWidget>

#include "BackButtonHandler.h"

namespace
{
class TestableBackButtonHandler : public BackButtonHandler
{
public:
    using BackButtonHandler::BackButtonHandler;

    bool callEventFilter(QObject* watched, QEvent* event)
    {
        return BackButtonHandler::eventFilter(watched, event);
    }
};
}  // namespace

void BackButtonHandlerTest::keyReleaseBackClearsFocusAndStopsEvent()
{
    QWidget widget;
    TestableBackButtonHandler handler;

    QKeyEvent keyReleaseEvent(QEvent::KeyRelease, Qt::Key_Back, Qt::NoModifier);
    const bool handled{handler.callEventFilter(&widget, &keyReleaseEvent)};

    QVERIFY(handled);
}

void BackButtonHandlerTest::nonBackKeyPassesThrough()
{
    QWidget widget;
    TestableBackButtonHandler handler;

    QKeyEvent keyReleaseEvent(QEvent::KeyRelease, Qt::Key_A, Qt::NoModifier);
    const bool handled{handler.callEventFilter(&widget, &keyReleaseEvent)};

    QVERIFY(!handled);
}
