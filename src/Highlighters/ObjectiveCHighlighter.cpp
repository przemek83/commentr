#include "ObjectiveCHighlighter.h"

#include "../Common.h"

ObjectiveCHighlighter::ObjectiveCHighlighter(const SpellChecker& spellChecker,
                                             QObject* parent)
    : CFamilyHighlighter(spellChecker, parent)
{
}

void ObjectiveCHighlighter::initRules()
{
    initFunctionsRules();

    for (const QString& pattern : keywords_)
        addRule(pattern, SyntaxElement::KEYWORD);

    addRule(classPattern_, SyntaxElement::CLASS);

    initQuotationRules();
}
