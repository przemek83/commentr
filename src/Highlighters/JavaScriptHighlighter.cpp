#include "JavaScriptHighlighter.h"

#include "../Common.h"

JavaScriptHighlighter::JavaScriptHighlighter(const SpellChecker& spellChecker,
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

void JavaScriptHighlighter::initRules()
{
    HighlightingRule rule;
    rule.startPattern_ =
        QRegularExpression(QStringLiteral(R"(\b[A-Za-z0-9_]+\s*(?=\())"));
    rule.format_ = Common::getFormat(SyntaxElement::FUNCTION);
    highlightingRules_.append(rule);

    for (const QString& pattern : keywords_)
    {
        rule.startPattern_ = QRegularExpression(pattern);
        rule.format_ = Common::getFormat(SyntaxElement::KEYWORD);
        highlightingRules_.append(rule);
    }

    rule.startPattern_ =
        QRegularExpression(QStringLiteral(R"(("([^"]|\\")*"|'([^']|\\')*'))"));
    rule.format_ = Common::getFormat(SyntaxElement::QUOTATION);
    highlightingRules_.append(rule);
}

void JavaScriptHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
    multiLineComment(text, multiLineCommentRule_);
}
