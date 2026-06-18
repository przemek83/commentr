#include "CSharpHighlighter.h"

CSharpHighlighter::CSharpHighlighter(const SpellChecker& spellChecker)
    : CFamilyHighlighter(spellChecker, QStringLiteral("cSharp.txt"))
{
}
