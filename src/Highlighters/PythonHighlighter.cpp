#include "PythonHighlighter.h"

#include "../Common.h"

PythonHighlighter::PythonHighlighter(const SpellChecker& spellChecker)
    : Highlighter(spellChecker)
{
    singleLineCommentRule_.format_ = Common::getFormat(SyntaxElement::COMMENT);
    singleLineCommentRule_.startPattern_ =
        QRegularExpression(singleLineCommentPattern_);
}

void PythonHighlighter::initRules()
{
    for (const QString& pattern : keywords_)
        addRule(pattern, SyntaxElement::KEYWORD);

    addRule(functionPattern_, SyntaxElement::FUNCTION);
    addRule(quotationPattern_, SyntaxElement::QUOTATION);
}

void PythonHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
}
