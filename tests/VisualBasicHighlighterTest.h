#ifndef VISUALBASICHIGHLIGHTERTEST_H
#define VISUALBASICHIGHLIGHTERTEST_H

#include <QObject>
#include <QTextDocument>

#include "SpellChecker.h"

class VisualBasicHighlighterTest : public QObject
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

#endif  // VISUALBASICHIGHLIGHTERTEST_H
