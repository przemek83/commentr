#include "PythonHighlighter.h"

#include "../Common.h"

PythonHighlighter::PythonHighlighter(const SpellChecker& spellChecker,
                                     QObject* parent)
    : Highlighter(spellChecker, parent)
{
    singleLineCommentRule_.format_ = Common::getFormat(SyntaxElement::COMMENT);
    singleLineCommentRule_.startPattern_ =
        QRegularExpression(QStringLiteral("#[^\n]*"));
}

void PythonHighlighter::initRules()
{
    HighlightingRule rule;
    for (const QString& pattern : keywords_)
    {
        rule.startPattern_ = QRegularExpression(pattern);
        rule.format_ = Common::getFormat(SyntaxElement::KEYWORD);
        highlightingRules_.append(rule);
    }

    rule.startPattern_ =
        QRegularExpression(QStringLiteral(R"(\b[A-Za-z0-9_]+\s*(?=\())"));
    rule.format_ = Common::getFormat(SyntaxElement::FUNCTION);
    highlightingRules_.append(rule);

    auto regexp(
        QStringLiteral(R"(("[^"]*"|'[^']*'|'''[^''']*'''|"""[^"""]*"""))"));
    rule.startPattern_ = QRegularExpression(regexp);
    rule.format_ = Common::getFormat(SyntaxElement::QUOTATION);
    highlightingRules_.append(rule);
}

void PythonHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
}
