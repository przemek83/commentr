#include "JavaHighlighterTest.h"

#include <QColor>
#include <QFont>
#include <QTest>
#include <QTextBlock>
#include <QTextDocument>
#include <QTextLayout>

#include "Common.h"
#include "Highlighters/JavaHighlighter.h"
#include "SpellChecker.h"

namespace
{
bool hasFormatForText(const QTextBlock& block, const QString& text,
                      const QTextCharFormat& expectedFormat,
                      bool italic = false)
{
    const qsizetype position{block.text().indexOf(text)};
    if (position < 0)
        return false;

    QTextLayout* layout{block.layout()};
    if (layout == nullptr)
        return false;

    for (const QTextLayout::FormatRange& range : layout->formats())
    {
        if ((range.start <= position) &&
            (position + text.length() <= range.start + range.length))
        {
            const QTextCharFormat& format{range.format};
            if ((format.foreground().color() ==
                 expectedFormat.foreground().color()) &&
                ((expectedFormat.fontWeight() < 0) ||
                 (format.fontWeight() == expectedFormat.fontWeight())) &&
                ((!italic) || format.fontItalic()))
            {
                return true;
            }
        }
    }

    return false;
}
}  // namespace

void JavaHighlighterTest::testKeywordHighlighting()
{
    SpellChecker spellChecker;
    spellChecker.setActive(false);

    QTextDocument document;
    JavaHighlighter highlighter(spellChecker, nullptr);

    const QString source{QStringLiteral("public class Foo")};
    document.setPlainText(source);
    highlighter.setDocument(&document);
    QCOMPARE(highlighter.document(), &document);
    highlighter.rehighlight();

    const QTextBlock block{document.firstBlock()};
    QCOMPARE(block.text(), source);

    QTextCharFormat keywordFormat{Common::getFormat(SyntaxElement::KEYWORD)};

    QVERIFY(hasFormatForText(block, QStringLiteral("public"), keywordFormat,
                             false));
    QVERIFY(
        hasFormatForText(block, QStringLiteral("class"), keywordFormat, false));
    QVERIFY(
        !hasFormatForText(block, QStringLiteral("Foo"), keywordFormat, false));
}

void JavaHighlighterTest::testQuotationAndCommentHighlighting()
{
    SpellChecker spellChecker;
    spellChecker.setActive(false);

    QTextDocument document;
    JavaHighlighter highlighter(spellChecker, nullptr);

    const QString source{QStringLiteral("String s = \"hello\"; // comment")};
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
    QVERIFY(hasFormatForText(block, QStringLiteral("// comment"), commentFormat,
                             false));
}
