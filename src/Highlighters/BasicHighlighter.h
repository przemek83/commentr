#ifndef BASICHIGHLIGHTER_H
#define BASICHIGHLIGHTER_H

#include "Highlighter.h"

class BasicHighlighter : public Highlighter
{
public:
    BasicHighlighter(const SpellChecker& spellChecker, QObject* parent);
    ~BasicHighlighter() override = default;

protected:
    void initRules() override;

    void commentBlock(const QString& text) override;

private:
    Q_DISABLE_COPY_MOVE(BasicHighlighter)

    HighlightingRule singleLineCommentRule_;
};

#endif  // BASICHIGHLIGHTER_H
