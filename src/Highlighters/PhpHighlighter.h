#ifndef PHPHIGHLIGHTER_H
#define PHPHIGHLIGHTER_H

#include "Highlighter.h"

class PhpHighlighter : public Highlighter
{
public:
    explicit PhpHighlighter(QObject* parent = 0);

protected:
    virtual void initRules();

    virtual void commentBlock(const QString& text);

private:
    Q_DISABLE_COPY(PhpHighlighter)

    HighlightingRule singleLineCommentRule_;

    HighlightingRule multiLineCommentRule_;
};

#endif  // PHPHIGHLIGHTER_H
