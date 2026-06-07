#ifndef CPLUSPLUSHIGHLIGHTER_H
#define CPLUSPLUSHIGHLIGHTER_H

#include "CFamilyHighlighter.h"

class CplusPlusHighlighter : public CFamilyHighlighter
{
public:
    explicit CplusPlusHighlighter(const SpellChecker& spellChecker);

protected:
    void initRules() override;

private:
    const QString qtClassPattern_{QStringLiteral("\\bQ[A-Za-z]+\\b")};
};

#endif  // CPLUSPLUSHIGHLIGHTER_H
