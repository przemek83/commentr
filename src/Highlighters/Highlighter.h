#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>

class SpellChecker;

class Highlighter : public QSyntaxHighlighter
{
public:
    explicit Highlighter(const SpellChecker& spellChecker, QObject* parent);

    ~Highlighter() override = default;

protected:
    void highlightBlock(const QString& text) override;

    virtual void initRules() = 0;

    struct HighlightingRule
    {
        QRegularExpression startPattern_;
        QRegularExpression endPattern_;
        QTextCharFormat format_;
    };

    QVector<HighlightingRule> highlightingRules_;

    virtual void commentBlock(const QString& text) = 0;

    void checkSpellingInBlock(int minIndex, const QString& line);

    static QStringList loadKeywords(const QString& fileName);

    bool initialized_{false};

    void singleLineComment(const QString& text, const HighlightingRule& rule);

    void multiLineComment(const QString& text, const HighlightingRule& rule);

private:
    Q_DISABLE_COPY_MOVE(Highlighter)

    void processWord(const QString& word, int minIndex, const QString& line);

    static constexpr int noMatchIndex_{-1};

    const SpellChecker& spellChecker_;
};

#endif  // HIGHLIGHTER_H
