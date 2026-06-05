#ifndef CSHARPHIGHLIGHTERTEST_H
#define CSHARPHIGHLIGHTERTEST_H

#include <QObject>
#include <QTextDocument>

#include "SpellChecker.h"

class CSharpHighlighterTest : public QObject
{
    Q_OBJECT

private:
    QTextDocument document_;
    SpellChecker spellChecker_;

private slots:
    void init();
    void testKeywordHighlighting();
    void testFunctionHighlighting();
    void testQuotationAndCommentHighlighting();
    void testMultilineCommentHighlighting();
};

#endif  // CSHARPHIGHLIGHTERTEST_H
