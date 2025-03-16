#ifndef PYTHONHIGHLIGHTER_H
#define PYTHONHIGHLIGHTER_H

#include "Highlighter.h"

class PythonHighlighter : public Highlighter
{
public:
    explicit PythonHighlighter(QObject* parent = 0);

protected:
    void initRules() override;

    void commentBlock(const QString& text) override;

private:
    Q_DISABLE_COPY(PythonHighlighter)

    //    QRegularExpression commentStartExpression_;

    //    QRegularExpression commentEndExpression_;

    //    QTextCharFormat multiLineCommentFormat_;

    HighlightingRule singleLineCommentRule_;

    HighlightingRule multiLineCommentRule_;
};

#endif  // PYTHONHIGHLIGHTER_H
