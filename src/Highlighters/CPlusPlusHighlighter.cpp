#include "CPlusPlusHighlighter.h"

CPlusPlusHighlighter::CPlusPlusHighlighter(const SpellChecker& spellChecker)
    : CFamilyHighlighter(spellChecker, QStringLiteral("cPlusPlus.txt"))
{
}

void CPlusPlusHighlighter::initRules()
{
    CFamilyHighlighter::initRules();
    addRule(qtClassPattern_, SyntaxElement::CLASS);
}
