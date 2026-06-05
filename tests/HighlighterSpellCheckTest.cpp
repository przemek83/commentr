#include "HighlighterSpellCheckTest.h"

#include <QTest>
#include <QTextBlock>
#include <QTextCharFormat>

#include "Common.h"
#include "HighlighterTestHelpers.h"

namespace
{
class TestHighlighter : public Highlighter
{
public:
    explicit TestHighlighter(const SpellChecker& spellChecker,
                             QObject* parent = nullptr)
        : Highlighter(spellChecker, parent)
    {
    }

    void initRules() override {}
    void commentBlock([[maybe_unused]] const QString& text) override {}

protected:
    void highlightBlock(const QString& text) override
    {
        // Call only the spelling check logic for this test
        checkSpellingInBlock(0, text);
    }
};
}  // namespace

void HighlighterSpellCheckTest::init() { spellChecker_.setActive(true); }

void HighlighterSpellCheckTest::testMisspelledWordIsMarked()
{
    spellChecker_.initDictionary(QStringLiteral("hello\nworld"));
    TestHighlighter highlighter(spellChecker_, nullptr);

    const QString source{QStringLiteral("hello badword world")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QVERIFY(hasFormatForText(block, QStringLiteral("badword"),
                             Common::getFormat(SyntaxElement::MISSPELLED_WORD),
                             false));
}

void HighlighterSpellCheckTest::testSingleLetterWordIgnored()
{
    spellChecker_.initDictionary(QStringLiteral("hello"));
    TestHighlighter highlighter(spellChecker_, nullptr);

    const QString source{QStringLiteral("x")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QVERIFY(!hasFormatForText(block, QStringLiteral("x"),
                              Common::getFormat(SyntaxElement::MISSPELLED_WORD),
                              false));
}

void HighlighterSpellCheckTest::testCorrectlySpelledWordNotMarked()
{
    spellChecker_.initDictionary(QStringLiteral("hello\nworld"));
    TestHighlighter highlighter(spellChecker_, nullptr);

    const QString source{QStringLiteral("hello world")};
    const QTextBlock block{setupHighlighter(highlighter, document_, source)};

    QVERIFY(!hasFormatForText(block, QStringLiteral("hello"),
                              Common::getFormat(SyntaxElement::MISSPELLED_WORD),
                              false));
}
