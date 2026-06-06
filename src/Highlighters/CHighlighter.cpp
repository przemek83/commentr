#include "CHighlighter.h"

#include "../Common.h"

CHighlighter::CHighlighter(const SpellChecker& spellChecker, QObject* parent)
    : CFamilyHighlighter(spellChecker, parent)
{
}

void CHighlighter::initRules()
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
