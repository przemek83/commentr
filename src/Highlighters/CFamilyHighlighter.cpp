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

void CFamilyHighlighter::initRules()
{
    addRule(functionPattern_, SyntaxElement::FUNCTION);

    for (const QString& pattern : keywords_)
        addRule(pattern, SyntaxElement::KEYWORD);

    addRule(quotationPattern_, SyntaxElement::QUOTATION);
}

void CFamilyHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
    multiLineComment(text, multiLineCommentRule_);
}
