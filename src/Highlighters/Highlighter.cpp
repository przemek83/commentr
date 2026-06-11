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
    qsizetype endIndex{endMatch.capturedStart()};
    qsizetype commentLength{0};
    if (endIndex == noMatchIndex_)
    {
        setCurrentBlockState(insideCommentBlockState_);
        commentLength = text.length() - startIndex;
    }
    else
    {
        commentLength = (endIndex - startIndex) + endMatch.capturedLength();
    }

    setFormat(startIndex, static_cast<int>(commentLength), rule.format_);
    checkSpellingInBlock(startIndex, text);
    QRegularExpressionMatch startMatch{
        rule.startPattern_.match(text, startIndex + commentLength)};
    startIndex = static_cast<int>(startMatch.capturedStart());
    return startIndex;
}
