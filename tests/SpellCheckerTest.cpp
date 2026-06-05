#include "SpellCheckerTest.h"

#include <QTest>

void SpellCheckerTest::init() { spellChecker_.setActive(true); }

void SpellCheckerTest::testDefaultActive()
{
    SpellChecker sc;
    QVERIFY(sc.active());
}

void SpellCheckerTest::testSetActive()
{
    SpellChecker sc;
    sc.setActive(false);
    QVERIFY(!sc.active());
    sc.setActive(true);
    QVERIFY(sc.active());
}

void SpellCheckerTest::testInitDictionaryAndCheckWord()
{
    SpellChecker sc;
    sc.initDictionary(QStringLiteral("hello\nworld\nfoo"));

    QVERIFY(sc.checkWord(QStringLiteral("hello")));
    QVERIFY(sc.checkWord(QStringLiteral("Foo")));
    QVERIFY(!sc.checkWord(QStringLiteral("bar")));
}
