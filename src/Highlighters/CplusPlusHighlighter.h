#ifndef CPLUSPLUSHIGHLIGHTER_H
#define CPLUSPLUSHIGHLIGHTER_H

#include "CFamilyHighlighter.h"

class CplusPlusHighlighter : public CFamilyHighlighter
{
public:
    CplusPlusHighlighter(const SpellChecker& spellChecker, QObject* parent);
    ~CplusPlusHighlighter() override = default;

protected:
    void initRules() override;

private:
    Q_DISABLE_COPY_MOVE(CplusPlusHighlighter)

    const QStringList keywords_{loadKeywords("cPlusPlus.txt")};

    const QString qtClassPattern_{QStringLiteral("\\bQ[A-Za-z]+\\b")};
};

#endif  // CPLUSPLUSHIGHLIGHTER_H
