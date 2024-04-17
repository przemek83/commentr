#include "JavaScriptHighlighter.h"

JavaScriptHighlighter::JavaScriptHighlighter(QObject * parent) :
    Highlighter(parent),
    commentStartExpression_(QRegularExpression("/\\*")),
    commentEndExpression_(QRegularExpression("\\*/"))
{
    multiLineCommentFormat_.setForeground(Qt::red);

    singleLineCommentRule_.format.setForeground(Qt::red);
    singleLineCommentRule_.pattern = QRegularExpression("//[^\n]*");
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
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+\\s*(?=\\()");
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
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules_.append(rule);
    }

    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("(\"([^\"]|\\\\\")*\"|\'([^\']|\\\\\')*\')");
    rule.format = quotationFormat;
    highlightingRules_.append(rule);
}

void JavaScriptHighlighter::commentBlock(const QString& text)
{
    //Single line comment.
    const QRegularExpression& expression = singleLineCommentRule_.pattern;
    QRegularExpressionMatch match = expression.match(text);
    if (match.hasMatch())
    {
        int length = match.capturedLength();
        int index = match.capturedStart();
        setFormat(index, length, singleLineCommentRule_.format);
        checkSpellingInBlock(index, text);
    }

    //Multi line comment.
    setCurrentBlockState(0);

    int startIndex = 0;
    if( previousBlockState() != 1 )
    {
        startIndex = commentStartExpression_.indexIn(text);
    }

    while (startIndex >= 0)
    {
        int endIndex = commentEndExpression_.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex
                            + commentEndExpression_.matchedLength();
        }

        setFormat(startIndex, commentLength, multiLineCommentFormat_);
        checkSpellingInBlock(startIndex, text);
        startIndex =
            commentStartExpression_.indexIn(text, startIndex + commentLength);
    }
}

