#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

class Highlighter : public QSyntaxHighlighter
{
public:
    explicit Highlighter(QObject *parent);

    virtual ~Highlighter();

    static void setSpellChecking(bool check);

protected:
    virtual void highlightBlock(const QString& text);

    virtual void initRules() = 0;

    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules_;

    virtual void commentBlock(const QString& text) = 0;

    void checkSpellingInBlock(int minIndex, const QString &line);

    bool initialized_;

private:
    Q_DISABLE_COPY(Highlighter)

    QTextCharFormat spellCheckFormat_;

    static bool spellChecking_;
};

#endif // HIGHLIGHTER_H
