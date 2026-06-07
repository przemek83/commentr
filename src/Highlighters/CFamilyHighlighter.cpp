#include "CFamilyHighlighter.h"

#include "../Common.h"

CFamilyHighlighter::CFamilyHighlighter(const SpellChecker& spellChecker,
                                       const QString& file)
    : Highlighter(spellChecker), keywords_{loadKeywords(file)}
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

void CFamilyHighlighter::initFunctionsRules()
{
    addRule(functionPattern_, SyntaxElement::FUNCTION);
}

void CFamilyHighlighter::initQuotationRules()
{
    addRule(quotationPattern_, SyntaxElement::QUOTATION);
}

void CFamilyHighlighter::initRules()
{
    initFunctionsRules();

    for (const QString& pattern : keywords_)
        addRule(pattern, SyntaxElement::KEYWORD);

    initQuotationRules();
}

void CFamilyHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
    multiLineComment(text, multiLineCommentRule_);
}
