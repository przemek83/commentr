#include "CHighlighterTest.h"

#include <QTest>

#include "Common.h"
#include "HighlighterTestHelpers.h"
#include "Highlighters/CHighlighter.h"

void CHighlighterTest::init() { spellChecker_.setActive(false); }

void CHighlighterTest::testKeywordHighlighting()
{
    CHighlighter highlighter(spellChecker_);

    const QString source{QStringLiteral("int main()")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QTextCharFormat keywordFormat{Common::getFormat(SyntaxElement::KEYWORD)};

    QVERIFY(
        hasFormatForText(block, QStringLiteral("int"), keywordFormat, false));
}

void CHighlighterTest::testFunctionHighlighting()
{
    CHighlighter highlighter(spellChecker_);

    const QString source{QStringLiteral("int main()")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QTextCharFormat functionFormat{Common::getFormat(SyntaxElement::FUNCTION)};
    QVERIFY(
        hasFormatForText(block, QStringLiteral("main"), functionFormat, true));
}

void CHighlighterTest::testQuotationAndCommentHighlighting()
{
    CHighlighter highlighter(spellChecker_);

    const QString source{QStringLiteral("char* s = \"hello\"; // comment")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QTextCharFormat quotationFormat{
        Common::getFormat(SyntaxElement::QUOTATION)};
    QTextCharFormat commentFormat{Common::getFormat(SyntaxElement::COMMENT)};

    QVERIFY(hasFormatForText(block, QStringLiteral("\"hello\""),
                             quotationFormat, false));
    QVERIFY(hasFormatForText(block, QStringLiteral("// comment"), commentFormat,
                             false));
}
