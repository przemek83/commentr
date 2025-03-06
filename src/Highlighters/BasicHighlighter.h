#ifndef BASICHIGHLIGHTER_H
#define BASICHIGHLIGHTER_H

#include "Highlighter.h"

class BasicHighlighter : public Highlighter
{
public:
    explicit  BasicHighlighter(QObject *parent = 0);

    virtual ~BasicHighlighter();

protected:
    virtual void initRules();

    virtual void commentBlock(const QString& text);

private:
    Q_DISABLE_COPY(BasicHighlighter)

    HighlightingRule singleLineCommentRule_;
};

#endif // BASICHIGHLIGHTER_H
