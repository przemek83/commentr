#include "PythonHighlighter.h"

PythonHighlighter::PythonHighlighter(QObject * parent) :
    Highlighter(parent)
{
    singleLineCommentRule_.format.setForeground(Qt::red);
    singleLineCommentRule_.pattern = QRegExp("#[^\n]*");
}

PythonHighlighter::~PythonHighlighter()
{

}

void PythonHighlighter::initRules()
{
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;

    keywordPatterns << "\\band\\b" << "\\bdel\\b" << "\\bfrom\\b" <<
        "\\bnot\\b" << "\\bwhile\\b" << "\\bas\\b" << "\\belif\\b" <<
        "\\bglobal\\b" << "\\bor\\b" << "\\bwith\\b" << "\\bassert\\b" <<
        "\\belse\\b" << "\\bif\\b" << "\\bpass\\b" << "\\byield\\b" <<
        "\\bbreak\\b" << "\\bexcept\\b" << "\\bimport\\b" << "\\bprint\\b" <<
        "\\bclass\\b" << "\\bexec\\b" << "\\bin\\b" << "\\braise\\b" <<
        "\\bcontinue\\b" << "\\bfinally\\b" << "\\bis\\b" << "\\breturn\\b" <<
        "\\bdef\\b" << "\\bfor\\b" << "\\blambda\\b" << "\\btry\\b";

    HighlightingRule rule;
    foreach (const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules_.append(rule);
    }

    QTextCharFormat functionFormat;
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+\\s*(?=\\()");
    rule.format = functionFormat;
    highlightingRules_.append(rule);

    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(Qt::darkGreen);
    QString regexp =
        QString("(\"[^\"]*\"|\'[^\']*\'|\'\'\'[^\'\'\']*\'\'\'|\"\"\"[^\"\"\"]*\"\"\")");
    rule.pattern = QRegExp(regexp);
    rule.format = quotationFormat;
    highlightingRules_.append(rule);
}

void PythonHighlighter::commentBlock(const QString& text)
{
    //Single line comment.
    const QRegExp& expression = singleLineCommentRule_.pattern;
    int index = expression.indexIn(text);
    if (index >= 0)
    {
        int length = expression.matchedLength();
        setFormat(index, length, singleLineCommentRule_.format);
        checkSpellingInBlock(index, text);
    }
}


