#ifndef OBJECTIVECHIGHLIGHTERTEST_H
#define OBJECTIVECHIGHLIGHTERTEST_H

#include <QObject>
#include <QTextDocument>

#include "SpellChecker.h"

class ObjectiveCHighlighterTest : public QObject
{
    Q_OBJECT

private:
    QTextDocument document_;
    SpellChecker spellChecker_;

private slots:
    void init();
    void testKeywordHighlighting();
    void testClassHighlighting();
    void testQuotationAndCommentHighlighting();
    void testMultilineCommentHighlighting();
};

#endif  // OBJECTIVECHIGHLIGHTERTEST_H
