#include "PythonHighlighter.h"

PythonHighlighter::PythonHighlighter(const SpellChecker& spellChecker,
                                     QObject* parent)
    : Highlighter(spellChecker, parent)
{
    singleLineCommentRule_.format_.setForeground(Qt::red);
    singleLineCommentRule_.startPattern_ =
        QRegularExpression(QStringLiteral("#[^\n]*"));
}

void PythonHighlighter::initRules()
{
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);

    HighlightingRule rule;
    for (const QString& pattern : keywords_)
    {
        rule.startPattern_ = QRegularExpression(pattern);
        rule.format_ = keywordFormat;
        highlightingRules_.append(rule);
    }

    QTextCharFormat functionFormat;
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.startPattern_ =
        QRegularExpression(QStringLiteral(R"(\b[A-Za-z0-9_]+\s*(?=\())"));
    rule.format_ = functionFormat;
    highlightingRules_.append(rule);

    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(Qt::darkGreen);
    QString regexp(
        QStringLiteral(R"(("[^"]*"|'[^']*'|'''[^''']*'''|"""[^"""]*"""))"));
    rule.startPattern_ = QRegularExpression(regexp);
    rule.format_ = quotationFormat;
    highlightingRules_.append(rule);
}

void PythonHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
}
