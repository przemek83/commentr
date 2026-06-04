#include "CSharpHighlighter.h"

CSharpHighlighter::CSharpHighlighter(const SpellChecker& spellChecker,
                                     QObject* parent)
    : CFamilyHighlighter(spellChecker, parent)
{
}

void CSharpHighlighter::initRules()
{
    initFunctionsRules();

    HighlightingRule rule;

    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);

    for (const QString& pattern : keywords_)
    {
        rule.startPattern_ = QRegularExpression(pattern);
        rule.format_ = keywordFormat;
        highlightingRules_.append(rule);
    }

    initQuotationRules();
}
