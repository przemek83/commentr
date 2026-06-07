#include "JavaHighlighter.h"

JavaHighlighter::JavaHighlighter(const SpellChecker& spellChecker)
    : CFamilyHighlighter(spellChecker, "java.txt")
{
}
