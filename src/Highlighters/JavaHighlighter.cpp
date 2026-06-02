#include "JavaHighlighter.h"

JavaHighlighter::JavaHighlighter(const SpellChecker& spellChecker,
                                 QObject* parent)
    : CFamilyHighlighter(spellChecker, parent)
{
}

void JavaHighlighter::initRules()
{
    initFunctionsRules();

    HighlightingRule rule;

    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns
        << QStringLiteral("\\babstract\\b") << QStringLiteral("\\bcontinue\\b")
        << QStringLiteral("\\bfor\\b") << QStringLiteral("\\bnew\\b")
        << QStringLiteral("\\bswitch\\b") << QStringLiteral("\\bassert\\b")
        << QStringLiteral("\\bdefault\\b") << QStringLiteral("\\bgoto\\b")
        << QStringLiteral("\\bpackage\\b")
        << QStringLiteral("\\bsynchronized\\b")
        << QStringLiteral("\\bboolean\\b") << QStringLiteral("\\bdo\\b")
        << QStringLiteral("\\bif\\b") << QStringLiteral("\\bprivate\\b")
        << QStringLiteral("\\bthis\\b") << QStringLiteral("\\bbreak\\b")
        << QStringLiteral("\\bdouble\\b") << QStringLiteral("\\bimplements\\b")
        << QStringLiteral("\\bprotected\\b") << QStringLiteral("\\bthrow\\b")
        << QStringLiteral("\\bbyte\\b") << QStringLiteral("\\belse\\b")
        << QStringLiteral("\\bimport\\b") << QStringLiteral("\\bpublic\\b")
        << QStringLiteral("\\bthrows\\b") << QStringLiteral("\\bcase\\b")
        << QStringLiteral("\\benum\\b") << QStringLiteral("\\binstanceof\\b")
        << QStringLiteral("\\breturn\\b") << QStringLiteral("\\btransient\\b")
        << QStringLiteral("\\bcatch\\b") << QStringLiteral("\\bextends\\b")
        << QStringLiteral("\\bint\\b") << QStringLiteral("\\bshort\\b")
        << QStringLiteral("\\btry\\b") << QStringLiteral("\\bchar\\b")
        << QStringLiteral("\\bfinal\\b") << QStringLiteral("\\binterface\\b")
        << QStringLiteral("\\bstatic\\b") << QStringLiteral("\\bvoid\\b")
        << QStringLiteral("\\bclass\\b") << QStringLiteral("\\bfinally\\b")
        << QStringLiteral("\\blong\\b") << QStringLiteral("\\bstrictfp\\b")
        << QStringLiteral("\\bvolatile\\b") << QStringLiteral("\\bconst\\b")
        << QStringLiteral("\\bfloat\\b") << QStringLiteral("\\bnative\\b")
        << QStringLiteral("\\bsuper\\b") << QStringLiteral("\\bwhile\\b");

    for (const QString& pattern : keywordPatterns)
    {
        rule.startPattern_ = QRegularExpression(pattern);
        rule.format_ = keywordFormat;
        highlightingRules_.append(rule);
    }

    initQuotationRules();
}
