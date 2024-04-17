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

    QRegularExpression commentStartExpression_;

    QRegularExpression commentEndExpression_;

    QTextCharFormat multiLineCommentFormat_;

    HighlightingRule singleLineCommentRule_;
};

#endif // JAVASCRIPTHIGHLIGHTER_H
