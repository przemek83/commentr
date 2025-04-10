#ifndef PHPHIGHLIGHTER_H
#define PHPHIGHLIGHTER_H

#include "Highlighter.h"

class PhpHighlighter : public Highlighter
{
public:
    explicit PhpHighlighter(QObject* parent);

protected:
    void initRules() override;

    void commentBlock(const QString& text) override;

private:
    Q_DISABLE_COPY(PhpHighlighter)

    HighlightingRule singleLineCommentRule_;

    HighlightingRule multiLineCommentRule_;
};

#endif  // PHPHIGHLIGHTER_H
