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
