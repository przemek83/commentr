#ifndef JAVAHIGHLIGHTER_H
#define JAVAHIGHLIGHTER_H

#include "CFamilyHighlighter.h"

class JavaHighlighter : public CFamilyHighlighter
{
public:
    explicit JavaHighlighter(const SpellChecker& spellChecker);
};

#endif  // JAVAHIGHLIGHTER_H
