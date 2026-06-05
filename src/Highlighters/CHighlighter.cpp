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

    const QStringList keywords{getKeywords()};
    for (const QString& pattern : keywords)
    {
        rule.startPattern_ = QRegularExpression(pattern);
        rule.format_ = Common::getFormat(SyntaxElement::KEYWORD);
        highlightingRules_.append(rule);
    }

    initQuotationRules();
}

QStringList CHighlighter::getKeywords() const { return keywords_; }
