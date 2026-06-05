#include "CSharpHighlighterTest.h"

#include <QTest>

#include "Common.h"
#include "HighlighterTestHelpers.h"
#include "Highlighters/CSharpHighlighter.h"

void CSharpHighlighterTest::init() { spellChecker_.setActive(false); }

void CSharpHighlighterTest::testKeywordHighlighting()
{
    CSharpHighlighter highlighter(spellChecker_, nullptr);

    const QString source{QStringLiteral("public class Foo")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QTextCharFormat keywordFormat{Common::getFormat(SyntaxElement::KEYWORD)};

    QVERIFY(hasFormatForText(block, QStringLiteral("public"), keywordFormat,
                             false));
}

void CSharpHighlighterTest::testFunctionHighlighting()
{
    CSharpHighlighter highlighter(spellChecker_, nullptr);

    const QString source{QStringLiteral("void foo()")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};
    QTextCharFormat functionFormat{Common::getFormat(SyntaxElement::FUNCTION)};
    QVERIFY(
        hasFormatForText(block, QStringLiteral("foo"), functionFormat, true));
}

void CSharpHighlighterTest::testQuotationAndCommentHighlighting()
{
    CSharpHighlighter highlighter(spellChecker_, nullptr);

    const QString source{QStringLiteral("string s = \"hello\"; // comment")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QTextCharFormat quotationFormat{
        Common::getFormat(SyntaxElement::QUOTATION)};
    QTextCharFormat commentFormat{Common::getFormat(SyntaxElement::COMMENT)};

    QVERIFY(hasFormatForText(block, QStringLiteral("\"hello\""),
                             quotationFormat, false));
    QVERIFY(hasFormatForText(block, QStringLiteral("// comment"), commentFormat,
                             false));
}

void CSharpHighlighterTest::testMultilineCommentHighlighting()
{
    CSharpHighlighter highlighter(spellChecker_, nullptr);

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
