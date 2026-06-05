#include "JavaScriptHighlighterTest.h"

#include <QTest>

#include "Common.h"
#include "HighlighterTestHelpers.h"
#include "Highlighters/JavaScriptHighlighter.h"

void JavaScriptHighlighterTest::init() { spellChecker_.setActive(false); }

void JavaScriptHighlighterTest::testKeywordHighlighting()
{
    JavaScriptHighlighter highlighter(spellChecker_, nullptr);

    const QString source{QStringLiteral("var x = 1;")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QTextCharFormat keywordFormat{Common::getFormat(SyntaxElement::KEYWORD)};

    QVERIFY(
        hasFormatForText(block, QStringLiteral("var"), keywordFormat, false));
}

void JavaScriptHighlighterTest::testFunctionHighlighting()
{
    JavaScriptHighlighter highlighter(spellChecker_, nullptr);

    const QString source{QStringLiteral("foo()")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};
    QTextCharFormat functionFormat{Common::getFormat(SyntaxElement::FUNCTION)};
    QVERIFY(
        hasFormatForText(block, QStringLiteral("foo"), functionFormat, true));
}

void JavaScriptHighlighterTest::testQuotationAndCommentHighlighting()
{
    JavaScriptHighlighter highlighter(spellChecker_, nullptr);

    const QString source{QStringLiteral("var s = \"hello\"; // comment")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QTextCharFormat quotationFormat{
        Common::getFormat(SyntaxElement::QUOTATION)};
    QTextCharFormat commentFormat{Common::getFormat(SyntaxElement::COMMENT)};

    QVERIFY(hasFormatForText(block, QStringLiteral("\"hello\""),
                             quotationFormat, false));
    QVERIFY(hasFormatForText(block, QStringLiteral("// comment"), commentFormat,
                             false));
}

void JavaScriptHighlighterTest::testMultilineCommentHighlighting()
{
    JavaScriptHighlighter highlighter(spellChecker_, nullptr);

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
