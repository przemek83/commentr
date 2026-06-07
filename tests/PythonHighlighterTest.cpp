#include "PythonHighlighterTest.h"

#include <QTest>
#include <QTextBlock>
#include <QTextDocument>

#include "Common.h"
#include "HighlighterTestHelpers.h"
#include "Highlighters/PythonHighlighter.h"

void PythonHighlighterTest::init() { spellChecker_.setActive(false); }

void PythonHighlighterTest::testKeywordHighlighting()
{
    PythonHighlighter highlighter(spellChecker_);

    const QString source{QStringLiteral("def foo():")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};
    QCOMPARE(block.text(), source);

    QTextCharFormat keywordFormat{Common::getFormat(SyntaxElement::KEYWORD)};

    QVERIFY(
        hasFormatForText(block, QStringLiteral("def"), keywordFormat, false));
    QVERIFY(
        !hasFormatForText(block, QStringLiteral("foo"), keywordFormat, false));
}

void PythonHighlighterTest::testFunctionHighlighting()
{
    PythonHighlighter highlighter(spellChecker_);

    const QString source{QStringLiteral("def foo():")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QTextCharFormat functionFormat{Common::getFormat(SyntaxElement::FUNCTION)};
    QVERIFY(
        hasFormatForText(block, QStringLiteral("foo"), functionFormat, true));
}

void PythonHighlighterTest::testQuotationAndCommentHighlighting()
{
    PythonHighlighter highlighter(spellChecker_);

    const QString source{QStringLiteral("s = \"hello\"  # comment")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};
    QCOMPARE(block.text(), source);

    QTextCharFormat quotationFormat{
        Common::getFormat(SyntaxElement::QUOTATION)};
    QTextCharFormat commentFormat{Common::getFormat(SyntaxElement::COMMENT)};

    QVERIFY(hasFormatForText(block, QStringLiteral("\"hello\""),
                             quotationFormat, false));
    QVERIFY(hasFormatForText(block, QStringLiteral("# comment"), commentFormat,
                             false));
}
