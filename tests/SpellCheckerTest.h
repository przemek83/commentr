#ifndef SPELLCHECKERTEST_H
#define SPELLCHECKERTEST_H

#include <QObject>

#include "SpellChecker.h"

class SpellCheckerTest : public QObject
{
    Q_OBJECT

private:
    SpellChecker spellChecker_;

private slots:
    void init();
    static void testDefaultActive();
    static void testSetActive();
    static void testInitDictionaryAndCheckWord();
};

#endif  // SPELLCHECKERTEST_H
