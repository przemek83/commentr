#ifndef CFAMILYHIGHLIGHTER_H
#define CFAMILYHIGHLIGHTER_H

#include <QRegularExpression>

#include "Highlighter.h"

class CFamilyHighlighter : public Highlighter
{
public:
    CFamilyHighlighter(const SpellChecker& spellChecker, const QString& file);
    ~CFamilyHighlighter() override = default;

protected:
    void initRules() override;

    void commentBlock(const QString& text) override;

private:
    Q_DISABLE_COPY_MOVE(CFamilyHighlighter)

    HighlightingRule singleLineCommentRule_;

    HighlightingRule multiLineCommentRule_;

    const QStringList keywords_;

    const QString functionPattern_{
        QStringLiteral(R"(\b[A-Za-z0-9_]+\s*(?=\())")};
    const QString quotationPattern_{
        QStringLiteral(R"(("([^"]|\\")*"|'([^']|\\')*'))")};
    const QString singleLineCommentPattern_{QStringLiteral("//[^\n]*")};
    const QString multiLineCommentPatternStart_{QStringLiteral("/\\*")};
    const QString multiLineCommentPatternEnd_{QStringLiteral("\\*/")};
};

#endif  // CFAMILYHIGHLIGHTER_H
