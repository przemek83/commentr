#ifndef JAVASCRIPTHIGHLIGHTER_H
#define JAVASCRIPTHIGHLIGHTER_H

#include "Highlighter.h"

class JavaScriptHighlighter : public Highlighter
{
public:
    explicit  JavaScriptHighlighter(QObject *parent = 0);

    virtual ~JavaScriptHighlighter();

protected:
    virtual void initRules();

    virtual void commentBlock(const QString& text);

private:
    Q_DISABLE_COPY(JavaScriptHighlighter)

    HighlightingRule singleLineCommentRule_;

    HighlightingRule multiLineCommentRule_;
};

#endif // JAVASCRIPTHIGHLIGHTER_H
