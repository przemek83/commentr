#include "CplusPlusHighlighter.h"

CplusPlusHighlighter::CplusPlusHighlighter(const SpellChecker& spellChecker,
                                           QObject* parent)
    : CFamilyHighlighter(spellChecker, parent)
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
    keywordPatterns << QStringLiteral("\\balignas\\b") << QStringLiteral("\\balignof\\b") << QStringLiteral("\\band\\b")
                    << QStringLiteral("\\band_eq\\b") << QStringLiteral("\\basm\\b") << QStringLiteral("\\bauto\\b")
                    << QStringLiteral("\\bbitand\\b") << QStringLiteral("\\bbitor\\b") << QStringLiteral("\\bbool\\b")
                    << QStringLiteral("\\bbreak\\b") << QStringLiteral("\\bcase\\b") << QStringLiteral("\\bcatch\\b")
                    << QStringLiteral("\\bchar\\b") << QStringLiteral("\\bchar16_t\\b") << QStringLiteral("\\bchar32_t\\b")
                    << QStringLiteral("\\bclass\\b") << QStringLiteral("\\bcompl\\b") << QStringLiteral("\\bconst\\b")
                    << QStringLiteral("\\bconstexpr\\b") << QStringLiteral("\\bconst_cast\\b")
                    << QStringLiteral("\\bcontinue\\b") << QStringLiteral("\\bdecltype\\b") << QStringLiteral("\\bdefault\\b")
                    << QStringLiteral("\\bdelete\\b") << QStringLiteral("\\bdo\\b") << QStringLiteral("\\bdouble\\b")
                    << QStringLiteral("\\bdynamic_cast\\b") << QStringLiteral("\\belse\\b") << QStringLiteral("\\benum\\b")
                    << QStringLiteral("\\bexplicit\\b") << QStringLiteral("\\bexport\\b") << QStringLiteral("\\bextern\\b")
                    << QStringLiteral("\\bfalse\\b") << QStringLiteral("\\bfloat\\b") << QStringLiteral("\\bfor\\b")
                    << QStringLiteral("\\bfriend\\b") << QStringLiteral("\\bgoto\\b") << QStringLiteral("\\bif\\b")
                    << QStringLiteral("\\binline\\b") << QStringLiteral("\\bint\\b") << QStringLiteral("\\blong\\b")
                    << QStringLiteral("\\bmutable\\b") << QStringLiteral("\\bnamespace\\b") << QStringLiteral("\\bnew\\b")
                    << QStringLiteral("\\bnoexcept\\b") << QStringLiteral("\\bnot\\b") << QStringLiteral("\\bnot_eq\\b")
                    << QStringLiteral("\\bnullptr\\b") << QStringLiteral("\\boperator\\b") << QStringLiteral("\\bor\\b")
                    << QStringLiteral("\\bor_eq\\b") << QStringLiteral("\\bprivate\\b") << QStringLiteral("\\bprotected\\b")
                    << QStringLiteral("\\bpublic\\b") << QStringLiteral("\\bregister\\b")
                    << QStringLiteral("\\breinterpret_cast\\b") << QStringLiteral("\\breturn\\b")
                    << QStringLiteral("\\bshort\\b") << QStringLiteral("\\bsigned\\b") << QStringLiteral("\\bsizeof\\b")
                    << QStringLiteral("\\bstatic\\b") << QStringLiteral("\\bstatic_assert\\b")
                    << QStringLiteral("\\bstatic_cast\\b") << QStringLiteral("\\bstruct\\b") << QStringLiteral("\\bswitch\\b")
                    << QStringLiteral("\\btemplate\\b") << QStringLiteral("\\bthis\\b") << QStringLiteral("\\bthread_local\\b")
                    << QStringLiteral("\\bthrow\\b") << QStringLiteral("\\btrue\\b") << QStringLiteral("\\btry\\b")
                    << QStringLiteral("\\btypedef\\b") << QStringLiteral("\\btypeid\\b") << QStringLiteral("\\btypename\\b")
                    << QStringLiteral("\\bunion\\b") << QStringLiteral("\\bunsigned\\b") << QStringLiteral("\\busing\\b")
                    << QStringLiteral("\\bvirtual\\b") << QStringLiteral("\\bvoid\\b") << QStringLiteral("\\bvolatile\\b")
                    << QStringLiteral("\\bwchar_t\\b") << QStringLiteral("\\bwhile\\b") << QStringLiteral("\\bxor\\b")
                    << QStringLiteral("\\bxor_eq\\b");

    // Preprocessor keywords.
    keywordPatterns << QStringLiteral("(^|\\s)#if\\b") << QStringLiteral("(^|\\s)#define\\b")
                    << QStringLiteral("(^|\\s)#include\\b") << QStringLiteral("(^|\\s)#ifdef\\b")
                    << QStringLiteral("(^|\\s)#undef\\b") << QStringLiteral("(^|\\s)#ifndef\\b")
                    << QStringLiteral("(^|\\s)#endif\\b") << QStringLiteral("(^|\\s)#else\\b")
                    << QStringLiteral("(^|\\s)#elif\\b");

    for (const QString& pattern : keywordPatterns)
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
