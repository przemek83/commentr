#include "PythonHighlighterTest.h"

#include <QTest>
#include <QTextBlock>
#include <QTextDocument>

#include "Common.h"
#include "HighlighterTestHelpers.h"
#include "Highlighters/PythonHighlighter.h"
#include "SpellChecker.h"

void PythonHighlighterTest::testKeywordHighlighting()
{
    SpellChecker spellChecker;
    spellChecker.setActive(false);

    QTextDocument document;
    PythonHighlighter highlighter(spellChecker, nullptr);

    const QString source{QStringLiteral("def foo():")};
    document.setPlainText(source);
    highlighter.setDocument(&document);
    QCOMPARE(highlighter.document(), &document);
    highlighter.rehighlight();

    const QTextBlock block{document.firstBlock()};
    QCOMPARE(block.text(), source);

    QTextCharFormat keywordFormat{Common::getFormat(SyntaxElement::KEYWORD)};

    QVERIFY(
        hasFormatForText(block, QStringLiteral("def"), keywordFormat, false));
    QVERIFY(
        !hasFormatForText(block, QStringLiteral("foo"), keywordFormat, false));
}

void PythonHighlighterTest::testQuotationAndCommentHighlighting()
{
    SpellChecker spellChecker;
    spellChecker.setActive(false);

    QTextDocument document;
    PythonHighlighter highlighter(spellChecker, nullptr);

    const QString source{QStringLiteral("s = \"hello\"  # comment")};
    document.setPlainText(source);
    highlighter.setDocument(&document);
    QCOMPARE(highlighter.document(), &document);
    highlighter.rehighlight();

    const QTextBlock block{document.firstBlock()};
    QCOMPARE(block.text(), source);

    QTextCharFormat quotationFormat{
        Common::getFormat(SyntaxElement::QUOTATION)};
    QTextCharFormat commentFormat{Common::getFormat(SyntaxElement::COMMENT)};

    QVERIFY(hasFormatForText(block, QStringLiteral("\"hello\""),
                             quotationFormat, false));
    QVERIFY(hasFormatForText(block, QStringLiteral("# comment"), commentFormat,
                             false));
}
