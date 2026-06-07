#include "CHighlighter.h"

#include "../Common.h"

CHighlighter::CHighlighter(const SpellChecker& spellChecker, QObject* parent)
    : CFamilyHighlighter(spellChecker, parent)
{
}

void CHighlighter::initRules()
{
    initFunctionsRules();

    for (const QString& pattern : keywords_)
        addRule(pattern, SyntaxElement::KEYWORD);

    initQuotationRules();
}
