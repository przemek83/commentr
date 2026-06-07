#include "VisualBasicHighlighter.h"

#include "../Common.h"

VisualBasicHighlighter::VisualBasicHighlighter(const SpellChecker& spellChecker)
    : Highlighter(spellChecker, "visualBasic.txt")
{
    singleLineCommentRule_.format_ = Common::getFormat(SyntaxElement::COMMENT);
    singleLineCommentRule_.startPattern_ =
        QRegularExpression(singleLineCommentPattern_);
}

void VisualBasicHighlighter::initRules()
{
    addRule(functionPattern_, SyntaxElement::FUNCTION);
    Highlighter::initRules();
    addRule(quotationPattern_, SyntaxElement::QUOTATION);
}

void VisualBasicHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
}
