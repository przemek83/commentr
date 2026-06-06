#ifndef PYTHONHIGHLIGHTER_H
#define PYTHONHIGHLIGHTER_H

#include "Highlighter.h"

class PythonHighlighter : public Highlighter
{
public:
    PythonHighlighter(const SpellChecker& spellChecker, QObject* parent);
    ~PythonHighlighter() override = default;

protected:
    void initRules() override;

    void commentBlock(const QString& text) override;

private:
    Q_DISABLE_COPY_MOVE(PythonHighlighter)

    HighlightingRule singleLineCommentRule_;

    const QStringList keywords_{loadKeywords("python.txt")};

    const QString functionPattern_{
        QStringLiteral(R"(\b[A-Za-z0-9_]+\s*(?=\())")};
    const QString quotationPattern_{
        QStringLiteral(R"(("[^"]*"|'[^']*'|'''[^''']*'''|"""[^"""]*"""))")};
    const QString singleLineCommentPattern_{QStringLiteral("#[^\n]*")};
};

#endif  // PYTHONHIGHLIGHTER_H
