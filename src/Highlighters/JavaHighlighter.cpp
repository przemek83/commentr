#include "JavaHighlighter.h"

JavaHighlighter::JavaHighlighter(const SpellChecker& spellChecker)
    : CFamilyHighlighter(spellChecker, QStringLiteral("java.txt"))
{
}
