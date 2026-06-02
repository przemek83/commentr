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
    QStringList keywordPatterns;

    keywordPatterns
        << QStringLiteral("\\babstract\\b") << QStringLiteral("\\bevent\\b")
        << QStringLiteral("\\bnew\\b") << QStringLiteral("\\bstruct\\b")
        << QStringLiteral("\\bas\\b") << QStringLiteral("\\bexplicit\\b")
        << QStringLiteral("\\bnull\\b") << QStringLiteral("\\bswitch\\b")
        << QStringLiteral("\\bbase\\b") << QStringLiteral("\\bextern\\b")
        << QStringLiteral("\\bobject\\b") << QStringLiteral("\\bthis\\b")
        << QStringLiteral("\\bbool\\b") << QStringLiteral("\\bfalse\\b")
        << QStringLiteral("\\boperator\\b") << QStringLiteral("\\bthrow\\b")
        << QStringLiteral("\\bbreak\\b") << QStringLiteral("\\bfinally\\b")
        << QStringLiteral("\\bout\\b") << QStringLiteral("\\btrue\\b")
        << QStringLiteral("\\bbyte\\b") << QStringLiteral("\\bfixed\\b")
        << QStringLiteral("\\boverride\\b") << QStringLiteral("\\btry\\b")
        << QStringLiteral("\\bcase\\b") << QStringLiteral("\\bfloat\\b")
        << QStringLiteral("\\bparams\\b") << QStringLiteral("\\btypeof\\b")
        << QStringLiteral("\\bcatch\\b") << QStringLiteral("\\bfor\\b")
        << QStringLiteral("\\bprivate\\b") << QStringLiteral("\\buint\\b")
        << QStringLiteral("\\bchar\\b") << QStringLiteral("\\bforeach\\b")
        << QStringLiteral("\\bprotected\\b") << QStringLiteral("\\bulong\\b")
        << QStringLiteral("\\bchecked\\b") << QStringLiteral("\\bgoto\\b")
        << QStringLiteral("\\bpublic\\b") << QStringLiteral("\\bunchecked\\b")
        << QStringLiteral("\\bclass\\b") << QStringLiteral("\\bif\\b")
        << QStringLiteral("\\breadonly\\b") << QStringLiteral("\\bunsafe\\b")
        << QStringLiteral("\\bconst\\b") << QStringLiteral("\\bimplicit\\b")
        << QStringLiteral("\\bref\\b") << QStringLiteral("\\bushort\\b")
        << QStringLiteral("\\bcontinue\\b") << QStringLiteral("\\bin\\b")
        << QStringLiteral("\\breturn\\b") << QStringLiteral("\\busing\\b")
        << QStringLiteral("\\bdecimal\\b") << QStringLiteral("\\bint\\b")
        << QStringLiteral("\\bsbyte\\b") << QStringLiteral("\\bvirtual\\b")
        << QStringLiteral("\\bdefault\\b") << QStringLiteral("\\binterface\\b")
        << QStringLiteral("\\bsealed\\b") << QStringLiteral("\\bvolatile\\b")
        << QStringLiteral("\\bdelegate\\b") << QStringLiteral("\\binternal\\b")
        << QStringLiteral("\\bshort\\b") << QStringLiteral("\\bvoid\\b")
        << QStringLiteral("\\bdo\\b") << QStringLiteral("\\bis\\b")
        << QStringLiteral("\\bsizeof\\b") << QStringLiteral("\\bwhile\\b")
        << QStringLiteral("\\bdouble\\b") << QStringLiteral("\\block\\b")
        << QStringLiteral("\\bstackalloc\\b") << QStringLiteral("\\b \\b")
        << QStringLiteral("\\belse\\b") << QStringLiteral("\\blong\\b")
        << QStringLiteral("\\bstatic\\b") << QStringLiteral("\\b \\b")
        << QStringLiteral("\\benum\\b") << QStringLiteral("\\bnamespace\\b")
        << QStringLiteral("\\bstring\\b");

    // Preprocessor keywords.
    keywordPatterns << QStringLiteral("(^|\\s)#if\\b")
                    << QStringLiteral("(^|\\s)#define\\b")
                    << QStringLiteral("(^|\\s)#include\\b")
                    << QStringLiteral("(^|\\s)#ifdef\\b")
                    << QStringLiteral("(^|\\s)#undef\\b")
                    << QStringLiteral("(^|\\s)#ifndef\\b")
                    << QStringLiteral("(^|\\s)#endif\\b")
                    << QStringLiteral("(^|\\s)#else\\b")
                    << QStringLiteral("(^|\\s)#import\\b")
                    << QStringLiteral("(^|\\s)#warning\\b")
                    << QStringLiteral("(^|\\s)#error\\b")
                    << QStringLiteral("(^|\\s)#line\\b")
                    << QStringLiteral("(^|\\s)#region\\b")
                    << QStringLiteral("(^|\\s)#endregion\\b");

    for (const QString& pattern : keywordPatterns)
    {
        rule.startPattern_ = QRegularExpression(pattern);
        rule.format_ = keywordFormat;
        highlightingRules_.append(rule);
    }

    initQuotationRules();
}
