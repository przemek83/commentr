#include "CSharpHighlighter.h"

CSharpHighlighter::CSharpHighlighter(QObject* parent)
    : CFamilyHighlighter(parent)
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

    keywordPatterns << "\\babstract\\b" << "\\bevent\\b" << "\\bnew\\b"
                    << "\\bstruct\\b" << "\\bas\\b" << "\\bexplicit\\b"
                    << "\\bnull\\b" << "\\bswitch\\b" << "\\bbase\\b"
                    << "\\bextern\\b" << "\\bobject\\b" << "\\bthis\\b"
                    << "\\bbool\\b" << "\\bfalse\\b" << "\\boperator\\b"
                    << "\\bthrow\\b" << "\\bbreak\\b" << "\\bfinally\\b"
                    << "\\bout\\b" << "\\btrue\\b" << "\\bbyte\\b"
                    << "\\bfixed\\b" << "\\boverride\\b" << "\\btry\\b"
                    << "\\bcase\\b" << "\\bfloat\\b" << "\\bparams\\b"
                    << "\\btypeof\\b" << "\\bcatch\\b" << "\\bfor\\b"
                    << "\\bprivate\\b" << "\\buint\\b" << "\\bchar\\b"
                    << "\\bforeach\\b" << "\\bprotected\\b" << "\\bulong\\b"
                    << "\\bchecked\\b" << "\\bgoto\\b" << "\\bpublic\\b"
                    << "\\bunchecked\\b" << "\\bclass\\b" << "\\bif\\b"
                    << "\\breadonly\\b" << "\\bunsafe\\b" << "\\bconst\\b"
                    << "\\bimplicit\\b" << "\\bref\\b" << "\\bushort\\b"
                    << "\\bcontinue\\b" << "\\bin\\b" << "\\breturn\\b"
                    << "\\busing\\b" << "\\bdecimal\\b" << "\\bint\\b"
                    << "\\bsbyte\\b" << "\\bvirtual\\b" << "\\bdefault\\b"
                    << "\\binterface\\b" << "\\bsealed\\b" << "\\bvolatile\\b"
                    << "\\bdelegate\\b" << "\\binternal\\b" << "\\bshort\\b"
                    << "\\bvoid\\b" << "\\bdo\\b" << "\\bis\\b"
                    << "\\bsizeof\\b" << "\\bwhile\\b" << "\\bdouble\\b"
                    << "\\block\\b" << "\\bstackalloc\\b" << "\\b \\b"
                    << "\\belse\\b" << "\\blong\\b" << "\\bstatic\\b"
                    << "\\b \\b" << "\\benum\\b" << "\\bnamespace\\b"
                    << "\\bstring\\b";

    // Preprocessor keywords.
    keywordPatterns << "(^|\\s)#if\\b" << "(^|\\s)#define\\b"
                    << "(^|\\s)#include\\b" << "(^|\\s)#ifdef\\b"
                    << "(^|\\s)#undef\\b" << "(^|\\s)#ifndef\\b"
                    << "(^|\\s)#endif\\b" << "(^|\\s)#else\\b"
                    << "(^|\\s)#import\\b" << "(^|\\s)#warning\\b"
                    << "(^|\\s)#error\\b" << "(^|\\s)#line\\b"
                    << "(^|\\s)#region\\b" << "(^|\\s)#endregion\\b";

    foreach (const QString& pattern, keywordPatterns)
    {
        rule.startPattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules_.append(rule);
    }

    initQuotationRules();
}
