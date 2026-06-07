#ifndef SQLHIGHLIGHTER_H
#define SQLHIGHLIGHTER_H

#include "Highlighter.h"

class SQLHighlighter : public Highlighter
{
public:
    explicit SQLHighlighter(const SpellChecker& spellChecker);
    ~SQLHighlighter() override = default;

protected:
    void highlightBlock(const QString& text) override;

    void initRules() override;

    void commentBlock(const QString& text) override;

private:
    Q_DISABLE_COPY_MOVE(SQLHighlighter)

    HighlightingRule singleLineCommentRule_;

    HighlightingRule multiLineCommentRule_;

    const QStringList keywords_{loadKeywords("sql.txt")};

    const QString quotationPattern_{QStringLiteral(R"(("[^"]*"|'[^']*'))")};
    const QString singleLineCommentPattern_{QStringLiteral("--[^\n]*")};
    const QString multiLineCommentPatternStart_{QStringLiteral("/\\*")};
    const QString multiLineCommentPatternEnd_{QStringLiteral("\\*/")};
};

#endif  // SQLHIGHLIGHTER_H
