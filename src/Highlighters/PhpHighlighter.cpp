#include "PhpHighlighter.h"

PhpHighlighter::PhpHighlighter(const SpellChecker& spellChecker,
                               QObject* parent)
    : Highlighter(spellChecker, parent),
      keywords_{loadKeywordsFromFile(":/keywords/keywords/php.txt")}
{
    singleLineCommentRule_.format_.setForeground(Qt::red);
    singleLineCommentRule_.startPattern_ =
        QRegularExpression(QStringLiteral("(//[^\n]*|#[^\n]*)"));

    multiLineCommentRule_.startPattern_ =
        QRegularExpression(QStringLiteral("/\\*"));
    multiLineCommentRule_.endPattern_ =
        QRegularExpression(QStringLiteral("\\*/"));
    multiLineCommentRule_.format_.setForeground(Qt::red);
}

void PhpHighlighter::initRules()
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
    rule.startPattern_ =
        QRegularExpression(QStringLiteral(R"(("([^"]|\\")*"|'([^']|\\')*'))"));
    rule.format_ = quotationFormat;
    highlightingRules_.append(rule);
}

void PhpHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
    multiLineComment(text, multiLineCommentRule_);
}
