#include "CFamilyHighlighter.h"

CFamilyHighlighter::CFamilyHighlighter(QObject *parent) :
    Highlighter(parent),
    commentStartExpression_(QRegularExpression("/\\*")),
    commentEndExpression_(QRegularExpression("\\*/"))
{
    multiLineCommentFormat_.setForeground(Qt::red);

    singleLineCommentRule_.format.setForeground(Qt::red);
    singleLineCommentRule_.pattern = QRegularExpression("//[^\n]*");
}

CFamilyHighlighter::~CFamilyHighlighter()
{

}

void CFamilyHighlighter::initFunctionsRules()
{
    HighlightingRule rule;

    QTextCharFormat functionFormat;
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+\\s*(?=\\()");
    rule.format = functionFormat;
    highlightingRules_.append(rule);
}

void CFamilyHighlighter::initQuotationRules()
{
    HighlightingRule rule;

    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(Qt::darkGreen);
    //rule.pattern = QRegularExpression("(\"([^\"]|\\\\\")*\"|\'([^\']|\\\\\')*\')");
    rule.pattern = QRegularExpression("(\"([^\"]|\\\\\")*\"|\'([^\']|\\\\\')*\')");
    rule.format = quotationFormat;
    highlightingRules_.append(rule);
}

void CFamilyHighlighter::commentBlock(const QString& text)
{
    //Single line comment.
    const QRegularExpression& expression = singleLineCommentRule_.pattern;
    int index = expression.indexIn(text);
    if (index >= 0)
    {
        int length = expression.matchedLength();
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
