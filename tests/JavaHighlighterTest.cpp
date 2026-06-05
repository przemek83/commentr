#include "JavaHighlighterTest.h"

#include <QColor>
#include <QFont>
#include <QTest>
#include <QTextBlock>
#include <QTextDocument>
#include <QTextLayout>

#include "Highlighters/JavaHighlighter.h"
#include "SpellChecker.h"

namespace
{
bool hasFormatForText(const QTextBlock& block, const QString& text,
                      const QColor& color, int weight = -1, bool italic = false)
{
    const qsizetype position = block.text().indexOf(text);
    if (position < 0)
        return false;

    QTextLayout* layout = block.layout();
    if (layout == nullptr)
        return false;

    for (const QTextLayout::FormatRange& range : layout->formats())
    {
        if ((range.start <= position) &&
            (position + text.length() <= range.start + range.length))
        {
            const QTextCharFormat& format = range.format;
            if ((format.foreground().color() == color) &&
                ((weight < 0) || (format.fontWeight() == weight)) &&
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

    const QString source = QStringLiteral("public class Foo");
    document.setPlainText(source);
    highlighter.setDocument(&document);
    QCOMPARE(highlighter.document(), &document);
    highlighter.rehighlight();

    const QTextBlock block = document.firstBlock();
    QCOMPARE(block.text(), source);

    QVERIFY(hasFormatForText(block, QStringLiteral("public"), Qt::darkBlue,
                             QFont::Bold));
    QVERIFY(hasFormatForText(block, QStringLiteral("class"), Qt::darkBlue,
                             QFont::Bold));
    QVERIFY(!hasFormatForText(block, QStringLiteral("Foo"), Qt::darkBlue,
                              QFont::Bold));
}

void JavaHighlighterTest::testQuotationAndCommentHighlighting()
{
    SpellChecker spellChecker;
    spellChecker.setActive(false);

    QTextDocument document;
    JavaHighlighter highlighter(spellChecker, nullptr);

    const QString source = QStringLiteral("String s = \"hello\"; // comment");
    document.setPlainText(source);
    highlighter.setDocument(&document);
    QCOMPARE(highlighter.document(), &document);
    highlighter.rehighlight();

    const QTextBlock block = document.firstBlock();
    QCOMPARE(block.text(), source);

    QVERIFY(
        hasFormatForText(block, QStringLiteral("\"hello\""), Qt::darkGreen));
    QVERIFY(hasFormatForText(block, QStringLiteral("// comment"), Qt::red));
}
