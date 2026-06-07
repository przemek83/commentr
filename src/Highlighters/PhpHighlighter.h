#ifndef PHPHIGHLIGHTER_H
#define PHPHIGHLIGHTER_H

#include "Highlighter.h"

class PhpHighlighter : public Highlighter
{
public:
    explicit PhpHighlighter(const SpellChecker& spellChecker);
    ~PhpHighlighter() override = default;

protected:
    void initRules() override;

    void commentBlock(const QString& text) override;

private:
    Q_DISABLE_COPY_MOVE(PhpHighlighter)

    HighlightingRule singleLineCommentRule_;

    HighlightingRule multiLineCommentRule_;

    const QString functionPattern_{
        QStringLiteral(R"(\b[A-Za-z0-9_]+\s*(?=\())")};
    const QString quotationPattern_{
        QStringLiteral(R"(("([^"]|\\")*"|'([^']|\\')*'))")};
    const QString singleLineCommentPattern_{
        QStringLiteral("(//[^\n]*|#[^\n]*)")};
    const QString multiLineCommentPatternStart_{QStringLiteral("/\\*")};
    const QString multiLineCommentPatternEnd_{QStringLiteral("\\*/")};
};

#endif  // PHPHIGHLIGHTER_H
