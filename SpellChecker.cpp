#include <QStringList>
#include <QDebug>

#include "SpellChecker.h"

SpellChecker &SpellChecker::getInstance()
{
    static SpellChecker checker;

    return checker;
}

SpellChecker::SpellChecker()
{

}

SpellChecker::~SpellChecker()
{

}

bool SpellChecker::checkWord(QString& word)
{
    return dictionary_.contains(word.toLower());
}

void SpellChecker::initDictionary(const QString& dictionaryFile)
{
    dictionary_ = QSet<QString>::fromList(dictionaryFile.split('\n'));
}
