#include "SQLHighlighterTest.h"

#include <QTest>

#include "Common.h"
#include "HighlighterTestHelpers.h"
#include "Highlighters/SQLHighlighter.h"

void SQLHighlighterTest::init() { spellChecker_.setActive(false); }

void SQLHighlighterTest::testKeywordHighlighting()
{
    SQLHighlighter highlighter(spellChecker_);

    const QString source{QStringLiteral("SELECT * FROM table")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QTextCharFormat keywordFormat{Common::getFormat(SyntaxElement::KEYWORD)};

    QVERIFY(hasFormatForText(block, QStringLiteral("SELECT"), keywordFormat,
                             false));
}

void SQLHighlighterTest::testQuotationAndCommentHighlighting()
{
    SQLHighlighter highlighter(spellChecker_);

    const QString source{QStringLiteral("SELECT 'hello' -- comment")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QTextCharFormat quotationFormat{
        Common::getFormat(SyntaxElement::QUOTATION)};
    QTextCharFormat commentFormat{Common::getFormat(SyntaxElement::COMMENT)};

    QVERIFY(hasFormatForText(block, QStringLiteral("'hello'"), quotationFormat,
                             false));
    QVERIFY(hasFormatForText(block, QStringLiteral("-- comment"), commentFormat,
                             false));
}

void SQLHighlighterTest::testMultilineCommentHighlighting()
{
    SQLHighlighter highlighter(spellChecker_);

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
