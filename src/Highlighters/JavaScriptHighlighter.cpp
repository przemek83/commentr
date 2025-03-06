#include "JavaScriptHighlighter.h"

JavaScriptHighlighter::JavaScriptHighlighter(QObject *parent)
    : Highlighter(parent)
{
    singleLineCommentRule_.format.setForeground(Qt::red);
    singleLineCommentRule_.startPattern = QRegularExpression("//[^\n]*");

    multiLineCommentRule_.startPattern = QRegularExpression("/\\*");
    multiLineCommentRule_.endPattern = QRegularExpression("\\*/");
    multiLineCommentRule_.format.setForeground(Qt::red);
}

JavaScriptHighlighter::~JavaScriptHighlighter()
{

}

void JavaScriptHighlighter::initRules()
{
    HighlightingRule rule;
    QTextCharFormat functionFormat;
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.startPattern = QRegularExpression("\\b[A-Za-z0-9_]+\\s*(?=\\()");
    rule.format = functionFormat;
    highlightingRules_.append(rule);

    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;

    keywordPatterns << "\\break\\b" << "\\bcase\\b" << "\\bcatch\\b" <<
        "\\bcontinue\\b" << "\\bdebugger\\b" << "\\bdefault\\b" << "\\bdelete\\b" <<
        "\\bdo\\b" << "\\belse\\b" << "\\bfinally\\b" << "\\bfor\\b" <<
        "\\bfunction\\b" << "\\bif\\b" << "\\bin\\b" << "\\binstanceof\\b" <<
        "\\bnew\\b" << "\\breturn\\b" << "\\bswitch\\b" << "\\bthis\\b" <<
        "\\bthrow\\b" << "\\btry\\b" << "\\btypeof\\b" << "\\bvar\\b" <<
        "\\bvoid\\b" << "\\bwhile\\b" << "\\bwith\\b" << "\\bconst\\b" <<
        "\\benum\\b" << "\\bexport\\b" << "\\bextends\\b" << "\\bimport\\b" <<
        "\\bsuper\\b" << "\\babstract\\b" << "\\bboolean\\b" << "\\bbyte\\b" <<
        "\\bchar\\b" << "\\bclass\\b" << "\\bdouble\\b" << "\\bfinal\\b" <<
        "\\bfloat\\b" << "\\bgoto\\b" << "\\bimplements\\b" << "\\bint\\b" <<
        "\\binterface\\b" << "\\blong\\b" << "\\bnative\\b" << "\\bpackage\\b" <<
        "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b" << "\\bshort\\b" <<
        "\\bstatic\\b" << "\\bsynchronized\\b" << "\\bthrows\\b" <<
        "\\btransient\\b" << "\\blet\\b" << "\\byield\\b";

    foreach (const QString &pattern, keywordPatterns)
    {
        rule.startPattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules_.append(rule);
    }

    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(Qt::darkGreen);
    rule.startPattern = QRegularExpression("(\"([^\"]|\\\\\")*\"|\'([^\']|\\\\\')*\')");
    rule.format = quotationFormat;
    highlightingRules_.append(rule);
}

void JavaScriptHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
    multiLineComment(text, multiLineCommentRule_);
}

