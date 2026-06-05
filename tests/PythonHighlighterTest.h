#ifndef PYTHONHIGHLIGHTERTEST_H
#define PYTHONHIGHLIGHTERTEST_H

#include <QObject>

#include <QTextDocument>

#include "SpellChecker.h"

class PythonHighlighterTest : public QObject
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

#endif  // PYTHONHIGHLIGHTERTEST_H
