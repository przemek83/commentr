#include "SpellChecker.h"

#include <QStringList>

SpellChecker& SpellChecker::getInstance()
{
    static SpellChecker checker;

    return checker;
}

bool SpellChecker::checkWord(const QString& word) const
{
    return dictionary_.contains(word.toLower());
}

void SpellChecker::initDictionary(const QString& dictionaryFile)
{
    QStringList words = dictionaryFile.split('\n');
    dictionary_ = QSet<QString>(words.begin(), words.end());
}
