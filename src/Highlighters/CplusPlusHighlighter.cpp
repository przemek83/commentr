#include "CplusPlusHighlighter.h"

CplusPlusHighlighter::CplusPlusHighlighter(QObject* parent)
    : CFamilyHighlighter(parent)
{
}

void CplusPlusHighlighter::initRules()
{
    initFunctionsRules();

    HighlightingRule rule;

    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\balignas\\b" << "\\balignof\\b" << "\\band\\b"
                    << "\\band_eq\\b" << "\\basm\\b" << "\\bauto\\b"
                    << "\\bbitand\\b" << "\\bbitor\\b" << "\\bbool\\b"
                    << "\\bbreak\\b" << "\\bcase\\b" << "\\bcatch\\b"
                    << "\\bchar\\b" << "\\bchar16_t\\b" << "\\bchar32_t\\b"
                    << "\\bclass\\b" << "\\bcompl\\b" << "\\bconst\\b"
                    << "\\bconstexpr\\b" << "\\bconst_cast\\b"
                    << "\\bcontinue\\b" << "\\bdecltype\\b" << "\\bdefault\\b"
                    << "\\bdelete\\b" << "\\bdo\\b" << "\\bdouble\\b"
                    << "\\bdynamic_cast\\b" << "\\belse\\b" << "\\benum\\b"
                    << "\\bexplicit\\b" << "\\bexport\\b" << "\\bextern\\b"
                    << "\\bfalse\\b" << "\\bfloat\\b" << "\\bfor\\b"
                    << "\\bfriend\\b" << "\\bgoto\\b" << "\\bif\\b"
                    << "\\binline\\b" << "\\bint\\b" << "\\blong\\b"
                    << "\\bmutable\\b" << "\\bnamespace\\b" << "\\bnew\\b"
                    << "\\bnoexcept\\b" << "\\bnot\\b" << "\\bnot_eq\\b"
                    << "\\bnullptr\\b" << "\\boperator\\b" << "\\bor\\b"
                    << "\\bor_eq\\b" << "\\bprivate\\b" << "\\bprotected\\b"
                    << "\\bpublic\\b" << "\\bregister\\b"
                    << "\\breinterpret_cast\\b" << "\\breturn\\b"
                    << "\\bshort\\b" << "\\bsigned\\b" << "\\bsizeof\\b"
                    << "\\bstatic\\b" << "\\bstatic_assert\\b"
                    << "\\bstatic_cast\\b" << "\\bstruct\\b" << "\\bswitch\\b"
                    << "\\btemplate\\b" << "\\bthis\\b" << "\\bthread_local\\b"
                    << "\\bthrow\\b" << "\\btrue\\b" << "\\btry\\b"
                    << "\\btypedef\\b" << "\\btypeid\\b" << "\\btypename\\b"
                    << "\\bunion\\b" << "\\bunsigned\\b" << "\\busing\\b"
                    << "\\bvirtual\\b" << "\\bvoid\\b" << "\\bvolatile\\b"
                    << "\\bwchar_t\\b" << "\\bwhile\\b" << "\\bxor\\b"
                    << "\\bxor_eq\\b";

    // Preprocessor keywords.
    keywordPatterns << "(^|\\s)#if\\b" << "(^|\\s)#define\\b"
                    << "(^|\\s)#include\\b" << "(^|\\s)#ifdef\\b"
                    << "(^|\\s)#undef\\b" << "(^|\\s)#ifndef\\b"
                    << "(^|\\s)#endif\\b" << "(^|\\s)#else\\b"
                    << "(^|\\s)#elif\\b";

    foreach (const QString& pattern, keywordPatterns)
    {
        rule.startPattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules_.append(rule);
    }

    QTextCharFormat classFormat;
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.startPattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules_.append(rule);

    initQuotationRules();
}
