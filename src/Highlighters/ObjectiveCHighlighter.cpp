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

    HighlightingRule rule;

    for (const QString& pattern : keywords_)
    {
        rule.startPattern_ = QRegularExpression(pattern);
        rule.format_ = Common::getFormat(SyntaxElement::KEYWORD);
        highlightingRules_.append(rule);
    }

    rule.startPattern_ =
        QRegularExpression(QStringLiteral("\\bNS[A-Za-z]+\\b"));
    rule.format_ = Common::getFormat(SyntaxElement::CLASS);
    highlightingRules_.append(rule);

    initQuotationRules();
}
