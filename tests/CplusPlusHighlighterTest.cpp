#include "CplusPlusHighlighterTest.h"

#include <QTest>

#include "Common.h"
#include "HighlighterTestHelpers.h"
#include "Highlighters/CplusPlusHighlighter.h"

void CplusPlusHighlighterTest::init() { spellChecker_.setActive(false); }

void CplusPlusHighlighterTest::testKeywordHighlighting()
{
    CplusPlusHighlighter highlighter(spellChecker_, nullptr);

    const QString source{QStringLiteral("class Foo")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QTextCharFormat keywordFormat{Common::getFormat(SyntaxElement::KEYWORD)};

    QVERIFY(
        hasFormatForText(block, QStringLiteral("class"), keywordFormat, false));
}

void CplusPlusHighlighterTest::testClassHighlighting()
{
    CplusPlusHighlighter highlighter(spellChecker_, nullptr);

    const QString source{QStringLiteral("QWidget w;")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};
    QTextCharFormat classFormat{Common::getFormat(SyntaxElement::CLASS)};
    QVERIFY(
        hasFormatForText(block, QStringLiteral("QWidget"), classFormat, false));
}

void CplusPlusHighlighterTest::testQuotationAndCommentHighlighting()
{
    CplusPlusHighlighter highlighter(spellChecker_, nullptr);

    const QString source{
        QStringLiteral("std::string s = \"hello\"; // comment")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QTextCharFormat quotationFormat{
        Common::getFormat(SyntaxElement::QUOTATION)};
    QTextCharFormat commentFormat{Common::getFormat(SyntaxElement::COMMENT)};

    QVERIFY(hasFormatForText(block, QStringLiteral("\"hello\""),
                             quotationFormat, false));
    QVERIFY(hasFormatForText(block, QStringLiteral("// comment"), commentFormat,
                             false));
}
