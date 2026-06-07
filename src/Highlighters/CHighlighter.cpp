#include "CHighlighter.h"

CHighlighter::CHighlighter(const SpellChecker& spellChecker)
    : CFamilyHighlighter(spellChecker, "c.txt")
{
}
