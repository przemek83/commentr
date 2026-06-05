#ifndef HIGHLIGHTERSPELLCHECKTEST_H
#define HIGHLIGHTERSPELLCHECKTEST_H

#include <QObject>
#include <QTextDocument>

#include "Highlighters/Highlighter.h"
#include "SpellChecker.h"

class HighlighterSpellCheckTest : public QObject
{
    Q_OBJECT

private:
    QTextDocument document_;
    SpellChecker spellChecker_;

private slots:
    void init();
    void testMisspelledWordIsMarked();
    void testSingleLetterWordIgnored();
    void testCorrectlySpelledWordNotMarked();
};

#endif  // HIGHLIGHTERSPELLCHECKTEST_H
