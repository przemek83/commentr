#include "CFamilyHighlighter.h"

#include "../Common.h"

CFamilyHighlighter::CFamilyHighlighter(const SpellChecker& spellChecker,
                                       QObject* parent)
    : Highlighter(spellChecker, parent)
{
    singleLineCommentRule_.format_ = Common::getFormat(SyntaxElement::COMMENT);
    singleLineCommentRule_.startPattern_ =
        QRegularExpression(QStringLiteral("//[^\n]*"));

    multiLineCommentRule_.startPattern_ =
        QRegularExpression(QStringLiteral("/\\*"));
    multiLineCommentRule_.endPattern_ =
        QRegularExpression(QStringLiteral("\\*/"));
    multiLineCommentRule_.format_ = Common::getFormat(SyntaxElement::COMMENT);
}

void CFamilyHighlighter::initFunctionsRules()
{
    HighlightingRule rule;

    rule.startPattern_ =
        QRegularExpression(QStringLiteral(R"(\b[A-Za-z0-9_]+\s*(?=\())"));
    rule.format_ = Common::getFormat(SyntaxElement::FUNCTION);
    highlightingRules_.append(rule);
}

void CFamilyHighlighter::initQuotationRules()
{
    HighlightingRule rule;
    rule.startPattern_ =
        QRegularExpression(QStringLiteral(R"(("([^"]|\\")*"|'([^']|\\')*'))"));
    rule.format_ = Common::getFormat(SyntaxElement::QUOTATION);
    highlightingRules_.append(rule);
}

void CFamilyHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
    multiLineComment(text, multiLineCommentRule_);
}
