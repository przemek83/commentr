#include "SQLHighlighter.h"

#include "../Common.h"

SQLHighlighter::SQLHighlighter(const SpellChecker& spellChecker)
    : Highlighter(spellChecker, "sql.txt")
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

void SQLHighlighter::initRules()
{
    Highlighter::initRules();

    addRule(quotationPattern_, SyntaxElement::QUOTATION);
}

void SQLHighlighter::highlightBlock(const QString& text)
{
    QString lower(text.toLower());
    Highlighter::highlightBlock(lower);
}

void SQLHighlighter::commentBlock(const QString& text)
{
    singleLineComment(text, singleLineCommentRule_);
    multiLineComment(text, multiLineCommentRule_);
}
