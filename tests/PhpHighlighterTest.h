#ifndef PHPHIGHLIGHTERTEST_H
#define PHPHIGHLIGHTERTEST_H

#include <QObject>
#include <QTextDocument>

#include "SpellChecker.h"

class PhpHighlighterTest : public QObject
{
    Q_OBJECT

private:
    QTextDocument document_;
    SpellChecker spellChecker_;

private slots:
    void init();
    void testKeywordHighlighting();
    void testQuotationAndCommentHighlighting();
};

#endif  // PHPHIGHLIGHTERTEST_H
