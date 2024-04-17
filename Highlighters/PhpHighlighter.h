#ifndef PHPHIGHLIGHTER_H
#define PHPHIGHLIGHTER_H

#include "Highlighter.h"

class PhpHighlighter : public Highlighter
{
public:
    explicit  PhpHighlighter(QObject *parent = 0);

    virtual ~PhpHighlighter();

protected:
    virtual void initRules();

    virtual void commentBlock(const QString& text);

private:
    Q_DISABLE_COPY(PhpHighlighter)

    QRegularExpression commentStartExpression_;

    QRegularExpression commentEndExpression_;

    QTextCharFormat multiLineCommentFormat_;

    HighlightingRule singleLineCommentRule_;
};

#endif // PHPHIGHLIGHTER_H
