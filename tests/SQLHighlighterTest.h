#ifndef SQLHIGHLIGHTERTEST_H
#define SQLHIGHLIGHTERTEST_H

#include <QObject>
#include <QTextDocument>

#include "SpellChecker.h"

class SQLHighlighterTest : public QObject
{
    Q_OBJECT

private:
    QTextDocument document_;
    SpellChecker spellChecker_;

private slots:
    void init();
    void testKeywordHighlighting();
    void testQuotationAndCommentHighlighting();
    void testMultilineCommentHighlighting();
};

#endif  // SQLHIGHLIGHTERTEST_H
