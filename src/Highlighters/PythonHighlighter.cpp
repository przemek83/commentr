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
    QStringList keywordPatterns;

    keywordPatterns
        << QStringLiteral("\\band\\b") << QStringLiteral("\\bdel\\b")
        << QStringLiteral("\\bfrom\\b") << QStringLiteral("\\bnot\\b")
        << QStringLiteral("\\bwhile\\b") << QStringLiteral("\\bas\\b")
        << QStringLiteral("\\belif\\b") << QStringLiteral("\\bglobal\\b")
        << QStringLiteral("\\bor\\b") << QStringLiteral("\\bwith\\b")
        << QStringLiteral("\\bassert\\b") << QStringLiteral("\\belse\\b")
        << QStringLiteral("\\bif\\b") << QStringLiteral("\\bpass\\b")
        << QStringLiteral("\\byield\\b") << QStringLiteral("\\bbreak\\b")
        << QStringLiteral("\\bexcept\\b") << QStringLiteral("\\bimport\\b")
        << QStringLiteral("\\bprint\\b") << QStringLiteral("\\bclass\\b")
        << QStringLiteral("\\bexec\\b") << QStringLiteral("\\bin\\b")
        << QStringLiteral("\\braise\\b") << QStringLiteral("\\bcontinue\\b")
        << QStringLiteral("\\bfinally\\b") << QStringLiteral("\\bis\\b")
        << QStringLiteral("\\breturn\\b") << QStringLiteral("\\bdef\\b")
        << QStringLiteral("\\bfor\\b") << QStringLiteral("\\blambda\\b")
        << QStringLiteral("\\btry\\b");

    HighlightingRule rule;
    for (const QString& pattern : keywordPatterns)
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
