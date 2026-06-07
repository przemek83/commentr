#include "JavaHighlighter.h"

#include "../Common.h"

JavaHighlighter::JavaHighlighter(const SpellChecker& spellChecker,
                                 QObject* parent)
    : CFamilyHighlighter(spellChecker, parent)
{
}

void JavaHighlighter::initRules()
{
    initFunctionsRules();

    for (const QString& pattern : keywords_)
        addRule(pattern, SyntaxElement::KEYWORD);

    initQuotationRules();
}
