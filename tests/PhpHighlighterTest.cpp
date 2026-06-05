#include "PhpHighlighterTest.h"

#include <QTest>

#include "Common.h"
#include "HighlighterTestHelpers.h"
#include "Highlighters/PhpHighlighter.h"

void PhpHighlighterTest::init() { spellChecker_.setActive(false); }

void PhpHighlighterTest::testKeywordHighlighting()
{
    PhpHighlighter highlighter(spellChecker_, nullptr);

    const QString source{QStringLiteral("function foo()")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QTextCharFormat keywordFormat{Common::getFormat(SyntaxElement::KEYWORD)};

    QVERIFY(hasFormatForText(block, QStringLiteral("function"), keywordFormat,
                             false));
}

void PhpHighlighterTest::testFunctionHighlighting()
{
    PhpHighlighter highlighter(spellChecker_, nullptr);

    const QString source{QStringLiteral("function foo()")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QTextCharFormat functionFormat{Common::getFormat(SyntaxElement::FUNCTION)};
    QVERIFY(
        hasFormatForText(block, QStringLiteral("foo"), functionFormat, true));
}

void PhpHighlighterTest::testQuotationAndCommentHighlighting()
{
    PhpHighlighter highlighter(spellChecker_, nullptr);

    const QString source{QStringLiteral("$s = \"hello\"; // comment")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QTextCharFormat quotationFormat{
        Common::getFormat(SyntaxElement::QUOTATION)};
    QTextCharFormat commentFormat{Common::getFormat(SyntaxElement::COMMENT)};

    QVERIFY(hasFormatForText(block, QStringLiteral("\"hello\""),
                             quotationFormat, false));
    QVERIFY(hasFormatForText(block, QStringLiteral("// comment"), commentFormat,
                             false));
}
