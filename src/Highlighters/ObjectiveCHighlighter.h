#ifndef OBJECTIVECHIGHLIGHTER_H
#define OBJECTIVECHIGHLIGHTER_H

#include "CFamilyHighlighter.h"

class ObjectiveCHighlighter : public CFamilyHighlighter
{
public:
    explicit ObjectiveCHighlighter(const SpellChecker& spellChecker);

protected:
    void initRules() override;

private:
    const QString classPattern_{QStringLiteral("\\bNS[A-Za-z]+\\b")};
};

#endif  // OBJECTIVECHIGHLIGHTER_H
