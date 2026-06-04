#include "CHighlighter.h"

CHighlighter::CHighlighter(const SpellChecker& spellChecker, QObject* parent)
    : CFamilyHighlighter(spellChecker, parent)
{
}

void CHighlighter::initRules()
{
    initFunctionsRules();

    HighlightingRule rule;

    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);

    const QStringList keywords{getKeywords()};
    for (const QString& pattern : keywords)
    {
        rule.startPattern_ = QRegularExpression(pattern);
        rule.format_ = keywordFormat;
        highlightingRules_.append(rule);
    }

    QTextCharFormat classFormat;
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.startPattern_ = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
    rule.format_ = classFormat;
    highlightingRules_.append(rule);

    initQuotationRules();
}

QStringList CHighlighter::getKeywords() const { return keywords_; }
