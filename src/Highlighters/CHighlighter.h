#ifndef CHIGHLIGHTER_H
#define CHIGHLIGHTER_H

#include "CFamilyHighlighter.h"

class CHighlighter : public CFamilyHighlighter
{
public:
    CHighlighter(const SpellChecker& spellChecker, QObject* parent);
    ~CHighlighter() override = default;

protected:
    void initRules() override;

private:
    Q_DISABLE_COPY_MOVE(CHighlighter)

    const QStringList keywords_{loadKeywords("c.txt")};
};

#endif  // CHIGHLIGHTER_H
