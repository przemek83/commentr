#ifndef SPELLCHECKER_H
#define SPELLCHECKER_H

#include <QObject>
#include <QSet>

class SpellChecker
{
public:
    SpellChecker() = default;
    SpellChecker(SpellChecker&&) = default;

    ~SpellChecker() = default;

    SpellChecker& operator=(SpellChecker&&) = default;

    bool checkWord(const QString& word) const;

    void initDictionary(const QString& dictionaryFile);

private:
    Q_DISABLE_COPY(SpellChecker)

    QSet<QString> dictionary_;
};

#endif  // SPELLCHECKER_H
