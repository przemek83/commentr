#ifndef SQLHIGHLIGHTER_H
#define SQLHIGHLIGHTER_H

#include "Highlighter.h"

class SQLHighlighter : public Highlighter
{
public:
    explicit  SQLHighlighter(QObject *parent = 0);

    virtual ~SQLHighlighter();

protected:
    virtual void highlightBlock(const QString& text);

    virtual void initRules();

    virtual void commentBlock(const QString& text);

private:
    Q_DISABLE_COPY(SQLHighlighter)

    QRegularExpression commentStartExpression_;

    QRegularExpression commentEndExpression_;

    QTextCharFormat multiLineCommentFormat_;

    HighlightingRule singleLineCommentRule_;
};

#endif // SQLHIGHLIGHTER_H
