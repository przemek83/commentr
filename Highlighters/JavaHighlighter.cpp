#include "JavaHighlighter.h"

JavaHighlighter::JavaHighlighter(QObject * parent) :
    CFamilyHighlighter(parent)
{

}

JavaHighlighter::~JavaHighlighter()
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
    keywordPatterns << "\\babstract\\b" << "\\bcontinue\\b" << "\\bfor\\b" <<
                       "\\bnew\\b" << "\\bswitch\\b" <<"\\bassert\\b" <<
                       "\\bdefault\\b" << "\\bgoto\\b" << "\\bpackage\\b" <<
                       "\\bsynchronized\\b" <<"\\bboolean\\b" << "\\bdo\\b" <<
                       "\\bif\\b" << "\\bprivate\\b" << "\\bthis\\b" <<"\\bbreak\\b" <<
                       "\\bdouble\\b" << "\\bimplements\\b" << "\\bprotected\\b" <<
                       "\\bthrow\\b" <<"\\bbyte\\b" << "\\belse\\b" << "\\bimport\\b" <<
                       "\\bpublic\\b" << "\\bthrows\\b" <<"\\bcase\\b" <<
                       "\\benum\\b" << "\\binstanceof\\b" << "\\breturn\\b" <<
                       "\\btransient\\b" <<"\\bcatch\\b" << "\\bextends\\b" <<
                       "\\bint\\b" << "\\bshort\\b" << "\\btry\\b" <<"\\bchar\\b" <<
                       "\\bfinal\\b" << "\\binterface\\b" << "\\bstatic\\b" <<
                       "\\bvoid\\b" <<"\\bclass\\b" << "\\bfinally\\b" <<
                       "\\blong\\b" << "\\bstrictfp\\b" << "\\bvolatile\\b" <<
                       "\\bconst\\b" << "\\bfloat\\b" << "\\bnative\\b" <<
                       "\\bsuper\\b" << "\\bwhile\\b";

    foreach (const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules_.append(rule);
    }

    initQuotationRules();
}

