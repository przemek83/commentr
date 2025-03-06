#ifndef PYTHONHIGHLIGHTER_H
#define PYTHONHIGHLIGHTER_H

#include "Highlighter.h"

class PythonHighlighter : public Highlighter
{
public:
    explicit  PythonHighlighter(QObject *parent = 0);

    virtual ~PythonHighlighter();

protected:
    virtual void initRules();

    virtual void commentBlock(const QString& text);

private:
    Q_DISABLE_COPY(PythonHighlighter)

//    QRegularExpression commentStartExpression_;

//    QRegularExpression commentEndExpression_;

//    QTextCharFormat multiLineCommentFormat_;

    HighlightingRule singleLineCommentRule_;

    HighlightingRule multiLineCommentRule_;
};

#endif // PYTHONHIGHLIGHTER_H
