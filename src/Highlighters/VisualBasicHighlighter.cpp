#include "VisualBasicHighlighter.h"

#include "../Common.h"

VisualBasicHighlighter::VisualBasicHighlighter(const SpellChecker& spellChecker,
                                               QObject* parent)
    : Highlighter(spellChecker, parent)
{
    singleLineCommentRule_.format_ = Common::getFormat(SyntaxElement::COMMENT);
    singleLineCommentRule_.startPattern_ =
        QRegularExpression(singleLineCommentPattern_);
}

void VisualBasicHighlighter::initRules()
{
    for (const QString& pattern : keywords_)
        addRule(pattern, SyntaxElement::KEYWORD);

    addRule(functionPattern_, SyntaxElement::FUNCTION);
    addRule(quotationPattern_, SyntaxElement::QUOTATION);
}

void VisualBasicHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
}
