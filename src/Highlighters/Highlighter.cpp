#include "Highlighter.h"

#include "../SpellChecker.h"

Highlighter::Highlighter(const SpellChecker& spellChecker, QObject* parent)
    : QSyntaxHighlighter(parent),
      initialized_(false),
      spellChecker_(spellChecker)
{
    spellCheckFormat_.setForeground(Qt::red);
    spellCheckFormat_.setUnderlineColor(QColor(Qt::red));
    spellCheckFormat_.setUnderlineStyle(QTextCharFormat::WaveUnderline);
}

void Highlighter::highlightBlock(const QString& text)
{
    if (!initialized_)
    {
        initRules();
        initialized_ = true;
    }

    foreach (const HighlightingRule& rule, highlightingRules_)
    {
        const QRegularExpression& expression = rule.startPattern_;
        for (const auto& match : expression.globalMatch(text))
        {
            int length = match.capturedLength();
            int index = match.capturedStart();
            setFormat(index, length, rule.format_);
        }
    }

    // Add comments highlight.
    commentBlock(text);
}

void Highlighter::checkSpellingInBlock(int minIndex, const QString& line)
{
    if (!spellChecker_.active())
        return;

    QString str{line.simplified()};
    QStringList wordsList = str.split(
        QRegularExpression(R"(([^\w,^\\]|(?=\\))+)"), Qt::SkipEmptyParts);
    foreach (QString word, wordsList)
    {
        if (word.length() > 1 && !word.startsWith('\\') &&
            !spellChecker_.checkWord(word))
        {
            int l{-1};
            int number{0};
            number = line.count(QRegularExpression("\\b" + word + "\\b"));
            for (int j = 0; j < number; ++j)
            {
                l = line.indexOf(QRegularExpression("\\b" + word + "\\b"),
                                 minIndex + l + 1);
                if (l >= 0)
                {
                    setFormat(l, word.length(), spellCheckFormat_);
                }
            }
        }
    }
}

void Highlighter::singleLineComment(const QString& text,
                                    const HighlightingRule& rule)
{
    const QRegularExpression& expression = rule.startPattern_;
    QRegularExpressionMatch match = expression.match(text);
    if (match.hasMatch())
    {
        int length = match.capturedLength();
        int index = match.capturedStart();
        setFormat(index, length, rule.format_);
        checkSpellingInBlock(index, text);
    }
}

void Highlighter::multiLineComment(const QString& text,
                                   const HighlightingRule& rule)
{
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
    {
        QRegularExpressionMatch match = rule.startPattern_.match(text);
        startIndex = match.capturedStart();
    }

    while (startIndex >= 0)
    {
        QRegularExpressionMatch endMatch =
            rule.endPattern_.match(text, startIndex);
        int endIndex = endMatch.capturedStart();
        int commentLength{0};
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex + endMatch.capturedLength();
        }

        setFormat(startIndex, commentLength, rule.format_);
        checkSpellingInBlock(startIndex, text);
        QRegularExpressionMatch startMatch =
            rule.startPattern_.match(text, startIndex + commentLength);
        startIndex = startMatch.capturedStart();
    }
}
