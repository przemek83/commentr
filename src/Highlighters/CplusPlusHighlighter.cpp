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

    HighlightingRule rule;

    const QStringList keywords{getKeywords()};
    for (const QString& pattern : keywords)
    {
        rule.startPattern_ = QRegularExpression(pattern);
        rule.format_ = Common::getFormat(SyntaxElement::KEYWORD);
        highlightingRules_.append(rule);
    }

    // For Qt classes
    rule.startPattern_ = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
    rule.format_ = Common::getFormat(SyntaxElement::CLASS);
    highlightingRules_.append(rule);

    initQuotationRules();
}

QStringList CplusPlusHighlighter::getKeywords() const { return keywords_; }
