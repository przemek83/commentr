#ifndef CSHARPHIGHLIGHTER_H
#define CSHARPHIGHLIGHTER_H

#include "CFamilyHighlighter.h"

class CSharpHighlighter : public CFamilyHighlighter
{
public:
    explicit CSharpHighlighter(const SpellChecker& spellChecker);
};

#endif  // CSHARPHIGHLIGHTER_H
