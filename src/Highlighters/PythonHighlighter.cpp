#include "PythonHighlighter.h"

PythonHighlighter::PythonHighlighter(QObject* parent) : Highlighter(parent)
{
    singleLineCommentRule_.format.setForeground(Qt::red);
    singleLineCommentRule_.startPattern = QRegularExpression("#[^\n]*");
}

void PythonHighlighter::initRules()
{
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;

    keywordPatterns << "\\band\\b" << "\\bdel\\b" << "\\bfrom\\b" << "\\bnot\\b"
                    << "\\bwhile\\b" << "\\bas\\b" << "\\belif\\b"
                    << "\\bglobal\\b" << "\\bor\\b" << "\\bwith\\b"
                    << "\\bassert\\b" << "\\belse\\b" << "\\bif\\b"
                    << "\\bpass\\b" << "\\byield\\b" << "\\bbreak\\b"
                    << "\\bexcept\\b" << "\\bimport\\b" << "\\bprint\\b"
                    << "\\bclass\\b" << "\\bexec\\b" << "\\bin\\b"
                    << "\\braise\\b" << "\\bcontinue\\b" << "\\bfinally\\b"
                    << "\\bis\\b" << "\\breturn\\b" << "\\bdef\\b"
                    << "\\bfor\\b" << "\\blambda\\b" << "\\btry\\b";

    HighlightingRule rule;
    foreach (const QString& pattern, keywordPatterns)
    {
        rule.startPattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules_.append(rule);
    }

    QTextCharFormat functionFormat;
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.startPattern = QRegularExpression("\\b[A-Za-z0-9_]+\\s*(?=\\()");
    rule.format = functionFormat;
    highlightingRules_.append(rule);

    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(Qt::darkGreen);
    QString regexp(
        "(\"[^\"]*\"|\'[^\']*\'|\'\'\'[^\'\'\']*\'\'\'|\"\"\"[^\"\"\"]*"
        "\"\"\")");
    rule.startPattern = QRegularExpression(regexp);
    rule.format = quotationFormat;
    highlightingRules_.append(rule);
}

void PythonHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
}
