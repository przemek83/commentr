#ifndef VISUALBASICHIGHLIGHTER_H
#define VISUALBASICHIGHLIGHTER_H

#include "Highlighter.h"

class VisualBasicHighlighter : public Highlighter
{
public:
    VisualBasicHighlighter(const SpellChecker& spellChecker, QObject* parent);
    ~VisualBasicHighlighter() override = default;

protected:
    void initRules() override;

    void commentBlock(const QString& text) override;

private:
    Q_DISABLE_COPY_MOVE(VisualBasicHighlighter)

    HighlightingRule singleLineCommentRule_;

    const QStringList keywords_;
};

#endif  // VISUALBASICHIGHLIGHTER_H
