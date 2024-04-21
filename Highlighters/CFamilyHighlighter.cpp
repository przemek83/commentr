#include "CFamilyHighlighter.h"

CFamilyHighlighter::CFamilyHighlighter(QObject *parent)
    : Highlighter(parent)
{
    singleLineCommentRule_.format.setForeground(Qt::red);
    singleLineCommentRule_.startPattern = QRegularExpression("//[^\n]*");

    multiLineCommentRule_.startPattern = QRegularExpression("/\\*");
    multiLineCommentRule_.endPattern = QRegularExpression("\\*/");
    multiLineCommentRule_.format.setForeground(Qt::red);
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
    rule.startPattern = QRegularExpression("\\b[A-Za-z0-9_]+\\s*(?=\\()");
    rule.format = functionFormat;
    highlightingRules_.append(rule);
}

void CFamilyHighlighter::initQuotationRules()
{
    HighlightingRule rule;

    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(Qt::darkGreen);
    //rule.pattern = QRegularExpression("(\"([^\"]|\\\\\")*\"|\'([^\']|\\\\\')*\')");
    rule.startPattern = QRegularExpression("(\"([^\"]|\\\\\")*\"|\'([^\']|\\\\\')*\')");
    rule.format = quotationFormat;
    highlightingRules_.append(rule);
}

void CFamilyHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
    multiLineComment(text, multiLineCommentRule_);
}
