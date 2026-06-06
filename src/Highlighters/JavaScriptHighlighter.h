#ifndef JAVASCRIPTHIGHLIGHTER_H
#define JAVASCRIPTHIGHLIGHTER_H

#include "Highlighter.h"

class JavaScriptHighlighter : public Highlighter
{
public:
    explicit JavaScriptHighlighter(const SpellChecker& spellChecker,
                                   QObject* parent);
    ~JavaScriptHighlighter() override = default;

protected:
    void initRules() override;

    void commentBlock(const QString& text) override;

private:
    Q_DISABLE_COPY_MOVE(JavaScriptHighlighter)

    HighlightingRule singleLineCommentRule_;

    HighlightingRule multiLineCommentRule_;

    const QStringList keywords_{loadKeywords("javaScript.txt")};

    const QString functionPattern_{
        QStringLiteral(R"(\b[A-Za-z0-9_]+\s*(?=\())")};
    const QString quotationPattern_{
        QStringLiteral(R"(("([^"]|\\")*"|'([^']|\\')*'))")};
    const QString singleLineCommentPattern_{QStringLiteral("//[^\n]*")};
    const QString multiLineCommentPatternStart_{QStringLiteral("/\\*")};
    const QString multiLineCommentPatternEnd_{QStringLiteral("\\*/")};
};

#endif  // JAVASCRIPTHIGHLIGHTER_H
