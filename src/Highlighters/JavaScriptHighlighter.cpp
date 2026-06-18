#include "JavaScriptHighlighter.h"

#include "../Common.h"

JavaScriptHighlighter::JavaScriptHighlighter(const SpellChecker& spellChecker)
    : Highlighter(spellChecker, QStringLiteral("javaScript.txt"))
{
    singleLineCommentRule_.format_ = Common::getFormat(SyntaxElement::COMMENT);
    singleLineCommentRule_.startPattern_ =
        QRegularExpression(singleLineCommentPattern_);

    multiLineCommentRule_.startPattern_ =
        QRegularExpression(multiLineCommentPatternStart_);
    multiLineCommentRule_.endPattern_ =
        QRegularExpression(multiLineCommentPatternEnd_);
    multiLineCommentRule_.format_ = Common::getFormat(SyntaxElement::COMMENT);
}

void JavaScriptHighlighter::initRules()
{
    addRule(functionPattern_, SyntaxElement::FUNCTION);
    Highlighter::initRules();
    addRule(quotationPattern_, SyntaxElement::QUOTATION);
}

void JavaScriptHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
    multiLineComment(text, multiLineCommentRule_);
}
