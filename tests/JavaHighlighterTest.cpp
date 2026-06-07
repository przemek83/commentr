#include "JavaHighlighterTest.h"

#include <QColor>
#include <QTest>
#include <QTextBlock>
#include <QTextDocument>

#include "Common.h"
#include "HighlighterTestHelpers.h"
#include "Highlighters/JavaHighlighter.h"

void JavaHighlighterTest::init() { spellChecker_.setActive(false); }

void JavaHighlighterTest::testKeywordHighlighting()
{
    JavaHighlighter highlighter(spellChecker_);

    const QString source{QStringLiteral("public class Foo")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QTextCharFormat keywordFormat{Common::getFormat(SyntaxElement::KEYWORD)};

    QVERIFY(hasFormatForText(block, QStringLiteral("public"), keywordFormat,
                             false));
    QVERIFY(
        hasFormatForText(block, QStringLiteral("class"), keywordFormat, false));
    QVERIFY(
        !hasFormatForText(block, QStringLiteral("Foo"), keywordFormat, false));
}

void JavaHighlighterTest::testFunctionHighlighting()
{
    JavaHighlighter highlighter(spellChecker_);

    const QString source{QStringLiteral("void foo()")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};
    QTextCharFormat functionFormat{Common::getFormat(SyntaxElement::FUNCTION)};
    QVERIFY(
        hasFormatForText(block, QStringLiteral("foo"), functionFormat, true));
}

void JavaHighlighterTest::testQuotationAndCommentHighlighting()
{
    JavaHighlighter highlighter(spellChecker_);

    const QString source{QStringLiteral("String s = \"hello\"; // comment")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QTextCharFormat quotationFormat{
        Common::getFormat(SyntaxElement::QUOTATION)};
    QTextCharFormat commentFormat{Common::getFormat(SyntaxElement::COMMENT)};

    QVERIFY(hasFormatForText(block, QStringLiteral("\"hello\""),
                             quotationFormat, false));
    QVERIFY(hasFormatForText(block, QStringLiteral("// comment"), commentFormat,
                             false));
}

void JavaHighlighterTest::testMultilineCommentHighlighting()
{
    JavaHighlighter highlighter(spellChecker_);

    const QString source{QStringLiteral("/* start\nmiddle\nend */")};
    const QTextBlock first{setupHighlighter(highlighter, document_, source)};
    const QTextBlock second{first.next()};
    const QTextBlock third{second.next()};

    QTextCharFormat commentFormat{Common::getFormat(SyntaxElement::COMMENT)};

    QVERIFY(hasFormatForText(first, QStringLiteral("/* start"), commentFormat,
                             false));
    QVERIFY(hasFormatForText(second, QStringLiteral("middle"), commentFormat,
                             false));
    QVERIFY(hasFormatForText(third, QStringLiteral("end */"), commentFormat,
                             false));
}
