#ifndef PHPHIGHLIGHTER_H
#define PHPHIGHLIGHTER_H

#include "Highlighter.h"

class PhpHighlighter : public Highlighter
{
public:
    PhpHighlighter(const SpellChecker& spellChecker, QObject* parent);
    ~PhpHighlighter() override = default;

protected:
    void initRules() override;

    void commentBlock(const QString& text) override;

private:
    Q_DISABLE_COPY_MOVE(PhpHighlighter)

    HighlightingRule singleLineCommentRule_;

    HighlightingRule multiLineCommentRule_;
};

#endif  // PHPHIGHLIGHTER_H
