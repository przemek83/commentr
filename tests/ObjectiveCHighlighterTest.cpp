#include "ObjectiveCHighlighterTest.h"

#include <QTest>

#include "Common.h"
#include "HighlighterTestHelpers.h"
#include "Highlighters/ObjectiveCHighlighter.h"

void ObjectiveCHighlighterTest::init() { spellChecker_.setActive(false); }

void ObjectiveCHighlighterTest::testKeywordHighlighting()
{
    ObjectiveCHighlighter highlighter(spellChecker_, nullptr);

    const QString source{QStringLiteral("if (true)")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QTextCharFormat keywordFormat{Common::getFormat(SyntaxElement::KEYWORD)};

    QVERIFY(
        hasFormatForText(block, QStringLiteral("if"), keywordFormat, false));
}

void ObjectiveCHighlighterTest::testClassHighlighting()
{
    ObjectiveCHighlighter highlighter(spellChecker_, nullptr);

    const QString source{QStringLiteral("NSObject obj;")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};
    QTextCharFormat classFormat{Common::getFormat(SyntaxElement::CLASS)};
    QVERIFY(hasFormatForText(block, QStringLiteral("NSObject"), classFormat,
                             false));
}

void ObjectiveCHighlighterTest::testQuotationAndCommentHighlighting()
{
    ObjectiveCHighlighter highlighter(spellChecker_, nullptr);

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
