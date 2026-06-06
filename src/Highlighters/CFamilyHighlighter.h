#ifndef CFAMILYHIGHLIGHTER_H
#define CFAMILYHIGHLIGHTER_H

#include <QRegularExpression>

#include "Highlighter.h"

class CFamilyHighlighter : public Highlighter
{
public:
    CFamilyHighlighter(const SpellChecker& spellChecker, QObject* parent);
    ~CFamilyHighlighter() override = default;

protected:
    void commentBlock(const QString& text) override;

    void initFunctionsRules();

    void initQuotationRules();

private:
    Q_DISABLE_COPY_MOVE(CFamilyHighlighter)

    HighlightingRule singleLineCommentRule_;

    HighlightingRule multiLineCommentRule_;

    const QString functionPattern_{
        QStringLiteral(R"(\b[A-Za-z0-9_]+\s*(?=\())")};
    const QString quotationPattern_{
        QStringLiteral(R"(("([^"]|\\")*"|'([^']|\\')*'))")};
    const QString singleLineCommentPattern_{QStringLiteral("//[^\n]*")};
    const QString multiLineCommentPatternStart_{QStringLiteral("/\\*")};
    const QString multiLineCommentPatternEnd_{QStringLiteral("\\*/")};
};

#endif  // CFAMILYHIGHLIGHTER_H
