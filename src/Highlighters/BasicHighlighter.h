#ifndef BASICHIGHLIGHTER_H
#define BASICHIGHLIGHTER_H

#include "Highlighter.h"

class BasicHighlighter : public Highlighter
{
public:
    explicit BasicHighlighter(QObject* parent);

protected:
    void initRules() override;

    void commentBlock(const QString& text) override;

private:
    Q_DISABLE_COPY(BasicHighlighter)

    HighlightingRule singleLineCommentRule_;
};

#endif  // BASICHIGHLIGHTER_H
