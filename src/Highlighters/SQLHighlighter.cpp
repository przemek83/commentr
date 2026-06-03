#include "SQLHighlighter.h"

SQLHighlighter::SQLHighlighter(const SpellChecker& spellChecker,
                               QObject* parent)
    : Highlighter(spellChecker, parent),
      keywords_{loadKeywordsFromFile(":/keywords/keywords/sql.txt")}
{
    singleLineCommentRule_.format_.setForeground(Qt::red);
    singleLineCommentRule_.startPattern_ =
        QRegularExpression(QStringLiteral("--[^\n]*"));

    multiLineCommentRule_.startPattern_ =
        QRegularExpression(QStringLiteral("/\\*"));
    multiLineCommentRule_.endPattern_ =
        QRegularExpression(QStringLiteral("\\*/"));
    multiLineCommentRule_.format_.setForeground(Qt::red);
}

void SQLHighlighter::initRules()
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

    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(Qt::darkGreen);
    rule.startPattern_ =
        QRegularExpression(QStringLiteral(R"(("[^"]*"|'[^']*'))"));
    rule.format_ = quotationFormat;
    highlightingRules_.append(rule);
}

void SQLHighlighter::highlightBlock(const QString& text)
{
    QString lower(text.toLower());
    Highlighter::highlightBlock(lower);
}

void SQLHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
    multiLineComment(text, multiLineCommentRule_);
}
