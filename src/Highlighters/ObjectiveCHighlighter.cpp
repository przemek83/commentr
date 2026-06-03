#include "ObjectiveCHighlighter.h"

ObjectiveCHighlighter::ObjectiveCHighlighter(const SpellChecker& spellChecker,
                                             QObject* parent)
    : CFamilyHighlighter(spellChecker, parent)
{
}

void ObjectiveCHighlighter::initRules()
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

    QTextCharFormat classFormat;
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.startPattern_ =
        QRegularExpression(QStringLiteral("\\bNS[A-Za-z]+\\b"));
    rule.format_ = classFormat;
    highlightingRules_.append(rule);

    initQuotationRules();
}
