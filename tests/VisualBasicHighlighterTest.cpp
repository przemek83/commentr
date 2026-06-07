#include "VisualBasicHighlighterTest.h"

#include <QTest>

#include "Common.h"
#include "HighlighterTestHelpers.h"
#include "Highlighters/VisualBasicHighlighter.h"

void VisualBasicHighlighterTest::init() { spellChecker_.setActive(false); }

void VisualBasicHighlighterTest::testKeywordHighlighting()
{
    VisualBasicHighlighter highlighter(spellChecker_);

    const QString source{QStringLiteral("If True Then")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QTextCharFormat keywordFormat{Common::getFormat(SyntaxElement::KEYWORD)};

    QVERIFY(
        hasFormatForText(block, QStringLiteral("If"), keywordFormat, false));
}

void VisualBasicHighlighterTest::testFunctionHighlighting()
{
    VisualBasicHighlighter highlighter(spellChecker_);

    const QString source{QStringLiteral("foo()")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};
    QTextCharFormat functionFormat{Common::getFormat(SyntaxElement::FUNCTION)};
    QVERIFY(
        hasFormatForText(block, QStringLiteral("foo"), functionFormat, true));
}

void VisualBasicHighlighterTest::testQuotationAndCommentHighlighting()
{
    VisualBasicHighlighter highlighter(spellChecker_);

    const QString source{QStringLiteral("s = \"hello\" ' comment")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QTextCharFormat quotationFormat{
        Common::getFormat(SyntaxElement::QUOTATION)};
    QTextCharFormat commentFormat{Common::getFormat(SyntaxElement::COMMENT)};

    QVERIFY(hasFormatForText(block, QStringLiteral("\"hello\""),
                             quotationFormat, false));
    QVERIFY(hasFormatForText(block, QStringLiteral("' comment"), commentFormat,
                             false));
}
