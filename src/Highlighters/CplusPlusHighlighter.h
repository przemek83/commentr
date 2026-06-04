#ifndef CPLUSPLUSHIGHLIGHTER_H
#define CPLUSPLUSHIGHLIGHTER_H

#include "CHighlighter.h"

class CplusPlusHighlighter : public CHighlighter
{
public:
    CplusPlusHighlighter(const SpellChecker& spellChecker, QObject* parent);
    ~CplusPlusHighlighter() override = default;

protected:
    QStringList getKeywords() const override;

private:
    Q_DISABLE_COPY_MOVE(CplusPlusHighlighter)

    const QStringList keywords_{loadKeywords("cPlusPlus.txt")};
};

#endif  // CPLUSPLUSHIGHLIGHTER_H
