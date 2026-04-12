#include "CFamilyHighlighter.h"

CFamilyHighlighter::CFamilyHighlighter(const SpellChecker& spellChecker,
                                       QObject* parent)
    : Highlighter(spellChecker, parent)
{
    singleLineCommentRule_.format_.setForeground(Qt::red);
    singleLineCommentRule_.startPattern_ = QRegularExpression("//[^\n]*");

    multiLineCommentRule_.startPattern_ = QRegularExpression("/\\*");
    multiLineCommentRule_.endPattern_ = QRegularExpression("\\*/");
    multiLineCommentRule_.format_.setForeground(Qt::red);
}

void CFamilyHighlighter::initFunctionsRules()
{
    HighlightingRule rule;

    QTextCharFormat functionFormat;
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.startPattern_ = QRegularExpression(R"(\b[A-Za-z0-9_]+\s*(?=\())");
    rule.format_ = functionFormat;
    highlightingRules_.append(rule);
}

void CFamilyHighlighter::initQuotationRules()
{
    HighlightingRule rule;

    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(Qt::darkGreen);
    rule.startPattern_ = QRegularExpression(R"(("([^"]|\\")*"|'([^']|\\')*'))");
    rule.format_ = quotationFormat;
    highlightingRules_.append(rule);
}

void CFamilyHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
    multiLineComment(text, multiLineCommentRule_);
}
