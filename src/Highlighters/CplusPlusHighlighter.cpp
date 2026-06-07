#include "CplusPlusHighlighter.h"

#include "../Common.h"

CplusPlusHighlighter::CplusPlusHighlighter(const SpellChecker& spellChecker,
                                           QObject* parent)
    : CFamilyHighlighter(spellChecker, parent)
{
}

void CplusPlusHighlighter::initRules()
{
    initFunctionsRules();

    for (const QString& pattern : keywords_)
        addRule(pattern, SyntaxElement::KEYWORD);

    addRule(qtClassPattern_, SyntaxElement::CLASS);

    initQuotationRules();
}
