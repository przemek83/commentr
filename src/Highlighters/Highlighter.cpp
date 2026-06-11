#include "Highlighter.h"

#include <QFile>
#include <QTextStream>

#include "../Common.h"
#include "../SpellChecker.h"

Highlighter::Highlighter(const SpellChecker& spellChecker, const QString& file)
    : QSyntaxHighlighter(static_cast<QObject*>(nullptr)),
      spellChecker_(spellChecker),
      keywords_{loadKeywords(file)}
{
}

void Highlighter::highlightBlock(const QString& text)
{
    if (highlightingRules_.empty())
        initRules();

    for (const HighlightingRule& rule : highlightingRules_)
        applyRule(text, rule);

    commentBlock(text);
}

void Highlighter::initRules()
{
    for (const QString& pattern : keywords_)
        addRule(pattern, SyntaxElement::KEYWORD);
}

void Highlighter::checkSpellingInBlock(int minIndex, const QString& line)
{
    if (!spellChecker_.active())
        return;

    constexpr int minSpellcheckWordLength{1};
    const QStringList words{SpellChecker::extractWords(line)};
    for (const QString& word : words)
    {
        if ((word.length() > minSpellcheckWordLength) &&
            (!spellChecker_.checkWord(word)))
        {
            processWord(word, minIndex, line);
        }
    }
}

QStringList Highlighter::loadKeywords(const QString& fileName) const
{
    QFile file(QStringLiteral(":/keywords/keywords/") + fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return {};

    QStringList keywords;

    QTextStream in(&file);
    const QString specialSignPrefix{QStringLiteral("(^|\\s)")};
    while (!in.atEnd())
    {
        QString line{QString::fromUtf8(file.readLine()).trimmed()};
        if (!line.isEmpty())
        {
            QString prefix{wordBoundary_};
            if (!line.at(0).isLetter())
                prefix = specialSignPrefix;
            keywords.append(prefix + line + wordBoundary_);
        }
    }

    return keywords;
}

void Highlighter::singleLineComment(const QString& text,
                                    const HighlightingRule& rule)
{
    const QRegularExpression& expression{rule.startPattern_};
    QRegularExpressionMatch match{expression.match(text)};
    if (!match.hasMatch())
        return;

    int length{static_cast<int>(match.capturedLength())};
    int index{static_cast<int>(match.capturedStart())};
    setFormat(index, length, rule.format_);
    checkSpellingInBlock(index, text);
}

void Highlighter::multiLineComment(const QString& text,
                                   const HighlightingRule& rule)
{
    setCurrentBlockState(noCommentBlockState_);

    int startIndex{0};
    if (previousBlockState() != insideCommentBlockState_)
    {
        QRegularExpressionMatch match{rule.startPattern_.match(text)};
        startIndex = static_cast<int>(match.capturedStart());
    }

    while (startIndex >= 0)
        startIndex = processCommentMatch(text, rule, startIndex);
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
    int startIndex{noMatchIndex_};
    const QRegularExpression expression(wordBoundary_ + word + wordBoundary_);
    const qsizetype occurrenceCount{line.count(expression)};
    for (qsizetype occurrence{0}; occurrence < occurrenceCount; ++occurrence)
    {
        startIndex = static_cast<int>(
            line.indexOf(expression, minIndex + startIndex + 1));
        if (startIndex >= 0)
        {
            setFormat(startIndex, static_cast<int>(word.length()),
                      Common::getFormat(SyntaxElement::MISSPELLED_WORD));
        }
    }
}

void Highlighter::applyRule(const QString& text, const HighlightingRule& rule)
{
    for (const auto& match : rule.startPattern_.globalMatch(text))
    {
        int length{static_cast<int>(match.capturedLength())};
        int index{static_cast<int>(match.capturedStart())};
        setFormat(index, length, rule.format_);
    }
}

int Highlighter::processCommentMatch(const QString& text,
                                     const HighlightingRule& rule,
                                     int startIndex)
{
    QRegularExpressionMatch endMatch{rule.endPattern_.match(text, startIndex)};
    const int commentLength{getCommentLength(text, endMatch, startIndex)};

    if (!isStartCaptured(endMatch))
        setCurrentBlockState(insideCommentBlockState_);

    setFormat(startIndex, commentLength, rule.format_);
    checkSpellingInBlock(startIndex, text);

    const QRegularExpressionMatch startMatch{
        rule.startPattern_.match(text, startIndex + commentLength)};
    startIndex = static_cast<int>(startMatch.capturedStart());
    return startIndex;
}

int Highlighter::getCommentLength(const QString& text,
                                  const QRegularExpressionMatch& match,
                                  int startIndex)
{
    qsizetype commentLength{0};
    if (isStartCaptured(match))
    {
        const qsizetype index{match.capturedStart()};
        commentLength = (index - startIndex) + match.capturedLength();
    }
    else
    {
        commentLength = text.length() - startIndex;
    }

    return static_cast<int>(commentLength);
}

bool Highlighter::isStartCaptured(const QRegularExpressionMatch& match)
{
    const qsizetype index{match.capturedStart()};
    return index != noMatchIndex_;
}
