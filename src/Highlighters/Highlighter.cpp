#include "Highlighter.h"

#include "../SpellChecker.h"

bool Highlighter::spellChecking_ = true;

Highlighter::Highlighter(QObject* parent)
    : QSyntaxHighlighter(parent), initialized_(false)
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
        const QRegularExpression& expression = rule.startPattern;
        for (const auto& match : expression.globalMatch(text))
        {
            int length = match.capturedLength();
            int index = match.capturedStart();
            setFormat(index, length, rule.format);
        }
    }

    // Add comments highlight.
    commentBlock(text);
}

void Highlighter::checkSpellingInBlock(int minIndex, const QString& line)
{
    if (!spellChecking_)
        return;

    QString str{line.simplified()};
    QStringList wordsList = str.split(
        QRegularExpression("([^\\w,^\\\\]|(?=\\\\))+"), Qt::SkipEmptyParts);
    foreach (QString word, wordsList)
    {
        if (word.length() > 1 && !word.startsWith('\\') &&
            !SpellChecker::getInstance().checkWord(word))
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
    const QRegularExpression& expression = rule.startPattern;
    QRegularExpressionMatch match = expression.match(text);
    if (match.hasMatch())
    {
        int length = match.capturedLength();
        int index = match.capturedStart();
        setFormat(index, length, rule.format);
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
        QRegularExpressionMatch match = rule.startPattern.match(text);
        startIndex = match.capturedStart();
    }

    while (startIndex >= 0)
    {
        QRegularExpressionMatch endMatch =
            rule.endPattern.match(text, startIndex);
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

        setFormat(startIndex, commentLength, rule.format);
        checkSpellingInBlock(startIndex, text);
        QRegularExpressionMatch startMatch =
            rule.startPattern.match(text, startIndex + commentLength);
        startIndex = startMatch.capturedStart();
    }
}

void Highlighter::setSpellChecking(bool check) { spellChecking_ = check; }
