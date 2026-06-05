#ifndef JAVASCRIPTHIGHLIGHTERTEST_H
#define JAVASCRIPTHIGHLIGHTERTEST_H

#include <QObject>
#include <QTextDocument>

#include "SpellChecker.h"

class JavaScriptHighlighterTest : public QObject
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

#endif  // JAVASCRIPTHIGHLIGHTERTEST_H
