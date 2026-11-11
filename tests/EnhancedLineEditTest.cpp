#include "EnhancedLineEditTest.h"

#include <QEvent>
#include <QMouseEvent>
#include <QTest>

#include "Config.h"
#include "EnhancedLineEdit.h"

namespace
{
class TestableEnhancedLineEdit : public EnhancedLineEdit
{
public:
    using EnhancedLineEdit::EnhancedLineEdit;

    bool callEvent(QEvent* event) { return EnhancedLineEdit::event(event); }

    void callChangeEvent(QEvent* event)
    {
        EnhancedLineEdit::changeEvent(event);
    }
};
}  // namespace

void EnhancedLineEditTest::constructorSetsMarginsFromUiSize()
{
    Config config;
    config.setUiSize(10.0F);

    EnhancedLineEdit lineEdit(config, nullptr);

    QCOMPARE(lineEdit.textMargins().left(), 5);
    QCOMPARE(lineEdit.textMargins().right(), 5);
    QCOMPARE(lineEdit.textMargins().top(), 0);
    QCOMPARE(lineEdit.textMargins().bottom(), 0);
}

void EnhancedLineEditTest::mouseMoveEventIsIgnored()
{
    Config config;
    config.setUiSize(10.0F);

    TestableEnhancedLineEdit lineEdit(config, nullptr);
    QMouseEvent mouseMoveEvent(QEvent::MouseMove, QPointF(), QPointF(),
                               Qt::NoButton, Qt::NoButton, Qt::NoModifier);

    const bool handled = lineEdit.callEvent(&mouseMoveEvent);

    QVERIFY(handled);
    QVERIFY(mouseMoveEvent.isAccepted() == false);
}

void EnhancedLineEditTest::styleChangeEventRefreshesMargins()
{
    Config config;
    config.setUiSize(12.0F);

    TestableEnhancedLineEdit lineEdit(config, nullptr);
    QCOMPARE(lineEdit.textMargins().right(), 6);

    config.setUiSize(20.0F);
    QEvent styleChangeEvent(QEvent::StyleChange);

    lineEdit.callChangeEvent(&styleChangeEvent);

    QCOMPARE(lineEdit.textMargins().right(), 10);
}
