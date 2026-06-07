#ifndef CHIGHLIGHTER_H
#define CHIGHLIGHTER_H

#include "CFamilyHighlighter.h"

class CHighlighter : public CFamilyHighlighter
{
public:
    explicit CHighlighter(const SpellChecker& spellChecker);
};

#endif  // CHIGHLIGHTER_H
