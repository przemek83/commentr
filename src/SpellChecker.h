#ifndef SPELLCHECKER_H
#define SPELLCHECKER_H

#include <QObject>
#include <QSet>

class SpellChecker
{
public:
    static SpellChecker& getInstance();

    bool checkWord(const QString& word) const;

    void initDictionary(const QString& dictionaryFile);

private:
    Q_DISABLE_COPY(SpellChecker)

    SpellChecker() = default;

    QSet<QString> dictionary_;
};

#endif  // SPELLCHECKER_H
