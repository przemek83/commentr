#include "CFamilyHighlighter.h"

#include "../Common.h"

CFamilyHighlighter::CFamilyHighlighter(const SpellChecker& spellChecker,
                                       QObject* parent)
    : Highlighter(spellChecker, parent)
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

void CFamilyHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
    multiLineComment(text, multiLineCommentRule_);
}
