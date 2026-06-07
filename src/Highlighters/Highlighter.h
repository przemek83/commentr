#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>

#include "../SyntaxElement.h"

class SpellChecker;

class Highlighter : public QSyntaxHighlighter
{
public:
    Highlighter(const SpellChecker& spellChecker, const QString& file);

    ~Highlighter() override = default;

protected:
    void highlightBlock(const QString& text) override;

    virtual void initRules();

    struct HighlightingRule
    {
        QRegularExpression startPattern_;
        QRegularExpression endPattern_;
        QTextCharFormat format_;
    };

    virtual void commentBlock(const QString& text) = 0;

    void checkSpellingInBlock(int minIndex, const QString& line);

    static QStringList loadKeywords(const QString& fileName);

    void singleLineComment(const QString& text, const HighlightingRule& rule);

    void multiLineComment(const QString& text, const HighlightingRule& rule);

    void addRule(const QString& pattern, SyntaxElement element);

private:
    Q_DISABLE_COPY_MOVE(Highlighter)

    void processWord(const QString& word, int minIndex, const QString& line);

    static constexpr int noMatchIndex_{-1};

    const SpellChecker& spellChecker_;

    QVector<HighlightingRule> highlightingRules_;

    const QStringList keywords_;
};

#endif  // HIGHLIGHTER_H
