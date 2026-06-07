#include "PhpHighlighter.h"

#include "../Common.h"

PhpHighlighter::PhpHighlighter(const SpellChecker& spellChecker)
    : Highlighter(spellChecker, "php.txt")
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

void PhpHighlighter::initRules()
{
    addRule(functionPattern_, SyntaxElement::FUNCTION);
    Highlighter::initRules();
    addRule(quotationPattern_, SyntaxElement::QUOTATION);
}

void PhpHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
    multiLineComment(text, multiLineCommentRule_);
}
