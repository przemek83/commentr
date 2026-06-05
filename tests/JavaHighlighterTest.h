#ifndef JAVAHIGHLIGHTERTEST_H
#define JAVAHIGHLIGHTERTEST_H

#include <QObject>
#include <QTextDocument>

#include "SpellChecker.h"

class JavaHighlighterTest : public QObject
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

#endif  // JAVAHIGHLIGHTERTEST_H
