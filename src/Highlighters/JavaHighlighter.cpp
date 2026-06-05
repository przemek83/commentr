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

    HighlightingRule rule;
    for (const QString& pattern : keywords_)
    {
        rule.startPattern_ = QRegularExpression(pattern);
        rule.format_ = Common::getFormat(SyntaxElement::KEYWORD);
        highlightingRules_.append(rule);
    }

    initQuotationRules();
}
