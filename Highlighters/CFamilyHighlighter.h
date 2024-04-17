#ifndef CFAMILYHIGHLIGHTER_H
#define CFAMILYHIGHLIGHTER_H

#include "Highlighter.h"

class CFamilyHighlighter : public Highlighter
{
public:
    explicit CFamilyHighlighter(QObject *parent);

    virtual ~CFamilyHighlighter();

protected:
    virtual void commentBlock(const QString& text);

    void initFunctionsRules();

    void initQuotationRules();

private:
    Q_DISABLE_COPY(CFamilyHighlighter)

    QRegExp commentStartExpression_;

    QRegExp commentEndExpression_;

    QTextCharFormat multiLineCommentFormat_;

    HighlightingRule singleLineCommentRule_;
};

#endif // CFAMILYHIGHLIGHTER_H
