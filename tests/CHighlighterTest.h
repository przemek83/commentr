#ifndef CHIGHLIGHTERTEST_H
#define CHIGHLIGHTERTEST_H

#include <QObject>
#include <QTextDocument>

#include "SpellChecker.h"

class CHighlighterTest : public QObject
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
};

#endif  // CHIGHLIGHTERTEST_H
