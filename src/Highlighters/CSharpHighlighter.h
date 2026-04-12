#ifndef CSHARPHIGHLIGHTER_H
#define CSHARPHIGHLIGHTER_H

#include "CFamilyHighlighter.h"

class CSharpHighlighter : public CFamilyHighlighter
{
public:
    CSharpHighlighter(const SpellChecker& spellChecker, QObject* parent);
    ~CSharpHighlighter() override = default;

protected:
    void initRules() override;

private:
    Q_DISABLE_COPY_MOVE(CSharpHighlighter)
};

#endif  // CSHARPHIGHLIGHTER_H
