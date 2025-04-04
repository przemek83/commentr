#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>

class Highlighter : public QSyntaxHighlighter
{
public:
    explicit Highlighter(QObject* parent);

    static void setSpellChecking(bool check);

protected:
    void highlightBlock(const QString& text) override;

    virtual void initRules() = 0;

    struct HighlightingRule
    {
        QRegularExpression startPattern;
        QRegularExpression endPattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules_;

    virtual void commentBlock(const QString& text) = 0;

    void checkSpellingInBlock(int minIndex, const QString& line);

    bool initialized_;

    void singleLineComment(const QString& text, const HighlightingRule& rule);

    void multiLineComment(const QString& text, const HighlightingRule& rule);

private:
    Q_DISABLE_COPY(Highlighter)

    QTextCharFormat spellCheckFormat_;

    static bool spellChecking_;
};

#endif  // HIGHLIGHTER_H
