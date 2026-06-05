#ifndef CPLUSPLUSHIGHLIGHTERTEST_H
#define CPLUSPLUSHIGHLIGHTERTEST_H

#include <QObject>
#include <QTextDocument>

#include "SpellChecker.h"

class CplusPlusHighlighterTest : public QObject
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
};

#endif  // CPLUSPLUSHIGHLIGHTERTEST_H
