#include "SpellChecker.h"

// MSVC temp fix.
#include <iterator>

#include <QRegularExpression>
#include <QStringList>

bool SpellChecker::checkWord(const QString& word) const
{
    return dictionary_.contains(word.toLower());
}

void SpellChecker::initDictionary(const QString& dictionaryFile)
{
    QStringList words = dictionaryFile.split('\n');
    dictionary_ = QSet<QString>(words.begin(), words.end());
}

bool SpellChecker::active() const { return active_; }

void SpellChecker::setActive(bool active) { active_ = active; }

QStringList SpellChecker::extractWords(const QString& text)
{
    QString str{text.simplified()};
    QStringList wordsList = str.split(
        QRegularExpression(QStringLiteral(R"([^\p{L}]+)")), Qt::SkipEmptyParts);
    return wordsList;
}
