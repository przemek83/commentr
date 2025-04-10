#ifndef SQLHIGHLIGHTER_H
#define SQLHIGHLIGHTER_H

#include "Highlighter.h"

class SQLHighlighter : public Highlighter
{
public:
    explicit SQLHighlighter(QObject* parent);

protected:
    void highlightBlock(const QString& text) override;

    void initRules() override;

    void commentBlock(const QString& text) override;

private:
    Q_DISABLE_COPY(SQLHighlighter)

    HighlightingRule singleLineCommentRule_;

    HighlightingRule multiLineCommentRule_;
};

#endif  // SQLHIGHLIGHTER_H
