#include "CSharpHighlighter.h"

#include "../Common.h"

CSharpHighlighter::CSharpHighlighter(const SpellChecker& spellChecker,
                                     QObject* parent)
    : CFamilyHighlighter(spellChecker, parent)
{
}

void CSharpHighlighter::initRules()
{
    initFunctionsRules();

    for (const QString& pattern : keywords_)
        addRule(pattern, SyntaxElement::KEYWORD);

    initQuotationRules();
}
