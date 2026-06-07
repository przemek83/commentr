#include "CplusPlusHighlighter.h"

CplusPlusHighlighter::CplusPlusHighlighter(const SpellChecker& spellChecker)
    : CFamilyHighlighter(spellChecker, "cPlusPlus.txt")
{
}

void CplusPlusHighlighter::initRules()
{
    CFamilyHighlighter::initRules();
    addRule(qtClassPattern_, SyntaxElement::CLASS);
}
