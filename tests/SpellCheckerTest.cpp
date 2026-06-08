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

void SpellCheckerTest::testSimpleSplit()
{
    const QString source{QStringLiteral("hello badword world")};
    const QStringList words = SpellChecker::extractWords(source);
    const QStringList expected{QStringLiteral("hello"),
                               QStringLiteral("badword"),
                               QStringLiteral("world")};
    QCOMPARE(words, expected);
}

void SpellCheckerTest::testUnicodeAndPunctuation()
{
    const QString source{QStringLiteral("héllo, мир!")};
    const QStringList words = SpellChecker::extractWords(source);
    const QStringList expected{QStringLiteral("héllo"), QStringLiteral("мир")};
    QCOMPARE(words, expected);
}

void SpellCheckerTest::testNumbersRemoved()
{
    const QString source{QStringLiteral("123 abc 456")};
    const QStringList words = SpellChecker::extractWords(source);
    const QStringList expected{QStringLiteral("abc")};
    QCOMPARE(words, expected);
}

void SpellCheckerTest::testPlusPlus()
{
    const QString source{QStringLiteral("C++ is fun")};
    const QStringList words = SpellChecker::extractWords(source);
    const QStringList expected{QStringLiteral("C"), QStringLiteral("is"),
                               QStringLiteral("fun")};
    QCOMPARE(words, expected);
}

void SpellCheckerTest::testLeadingSymbols()
{
    const QString source{QStringLiteral("@test #foo")};
    const QStringList words = SpellChecker::extractWords(source);
    const QStringList expected{QStringLiteral("test"), QStringLiteral("foo")};
    QCOMPARE(words, expected);
}
