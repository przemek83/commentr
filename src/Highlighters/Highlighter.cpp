#include "Highlighter.h"

#include <QFile>
#include <QTextStream>

#include "../Common.h"
#include "../SpellChecker.h"

Highlighter::Highlighter(const SpellChecker& spellChecker)
    : QSyntaxHighlighter(static_cast<QObject*>(nullptr)),
      spellChecker_(spellChecker)
{
}

void Highlighter::highlightBlock(const QString& text)
{
    if (highlightingRules_.empty())
        initRules();

    for (const HighlightingRule& rule : highlightingRules_)
    {
        const QRegularExpression& expression = rule.startPattern_;
        for (const auto& match : expression.globalMatch(text))
        {
            int length{static_cast<int>(match.capturedLength())};
            int index{static_cast<int>(match.capturedStart())};
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

    constexpr int minSpellcheckWordLength{1};

    QString str{line.simplified()};
    QStringList wordsList =
        str.split(QRegularExpression(QStringLiteral(R"(([^\w,^\\]|(?=\\))+)")),
                  Qt::SkipEmptyParts);
    for (const QString& word : wordsList)
    {
        if ((word.length() > minSpellcheckWordLength) &&
            (!word.startsWith('\\')) && (!spellChecker_.checkWord(word)))
        {
            processWord(word, minIndex, line);
        }
    }
}

QStringList Highlighter::loadKeywords(const QString& fileName)
{
    QFile file(QStringLiteral(":/keywords/keywords/") + fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return {};

    QStringList keywords;

    QTextStream in(&file);
    const QString wordBoundary{QStringLiteral("\\b")};
    const QString specialSignPrefix{QStringLiteral("(^|\\s)")};
    while (!in.atEnd())
    {
        QString line{QString::fromUtf8(file.readLine()).trimmed()};
        if (!line.isEmpty())
        {
            QString prefix{wordBoundary};
            if (!line.at(0).isLetter())
                prefix = specialSignPrefix;
            keywords.append(prefix + line + wordBoundary);
        }
    }

    return keywords;
}

void Highlighter::singleLineComment(const QString& text,
                                    const HighlightingRule& rule)
{
    const QRegularExpression& expression{rule.startPattern_};
    QRegularExpressionMatch match{expression.match(text)};
    if (match.hasMatch())
    {
        int length{static_cast<int>(match.capturedLength())};
        int index{static_cast<int>(match.capturedStart())};
        setFormat(index, length, rule.format_);
        checkSpellingInBlock(index, text);
    }
}

void Highlighter::multiLineComment(const QString& text,
                                   const HighlightingRule& rule)
{
    constexpr int noCommentBlockState{0};
    constexpr int insideCommentBlockState{1};

    setCurrentBlockState(noCommentBlockState);

    int startIndex = 0;
    if (previousBlockState() != insideCommentBlockState)
    {
        QRegularExpressionMatch match = rule.startPattern_.match(text);
        startIndex = static_cast<int>(match.capturedStart());
    }

    while (startIndex >= 0)
    {
        QRegularExpressionMatch endMatch =
            rule.endPattern_.match(text, startIndex);
        auto endIndex = static_cast<int>(endMatch.capturedStart());
        int commentLength{0};
        if (endIndex == noMatchIndex_)
        {
            setCurrentBlockState(insideCommentBlockState);
            commentLength = static_cast<int>(text.length() - startIndex);
        }
        else
        {
            commentLength = (endIndex - startIndex) +
                            static_cast<int>(endMatch.capturedLength());
        }

        setFormat(startIndex, commentLength, rule.format_);
        checkSpellingInBlock(startIndex, text);
        QRegularExpressionMatch startMatch =
            rule.startPattern_.match(text, startIndex + commentLength);
        startIndex = static_cast<int>(startMatch.capturedStart());
    }
}

void Highlighter::addRule(const QString& pattern, SyntaxElement element)
{
    HighlightingRule rule;
    rule.startPattern_ = QRegularExpression(pattern);
    rule.format_ = Common::getFormat(element);
    highlightingRules_.append(rule);
}

void Highlighter::processWord(const QString& word, int minIndex,
                              const QString& line)
{
    int l{noMatchIndex_};
    int number{static_cast<int>(line.count(QRegularExpression(
        QLatin1String("\\b") + word + QLatin1String("\\b"))))};
    for (int j = 0; j < number; ++j)
    {
        l = static_cast<int>(
            line.indexOf(QRegularExpression(QLatin1String("\\b") + word +
                                            QLatin1String("\\b")),
                         minIndex + l + 1));
        if (l >= 0)
        {
            setFormat(l, static_cast<int>(word.length()),
                      Common::getFormat(SyntaxElement::MISSPELLED_WORD));
        }
    }
}
