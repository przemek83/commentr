#ifndef OBJECTIVECHIGHLIGHTER_H
#define OBJECTIVECHIGHLIGHTER_H

#include "CFamilyHighlighter.h"

class ObjectiveCHighlighter : public CFamilyHighlighter
{
public:
    ObjectiveCHighlighter(const SpellChecker& spellChecker, QObject* parent);
    ~ObjectiveCHighlighter() override = default;

protected:
    void initRules() override;

private:
    Q_DISABLE_COPY_MOVE(ObjectiveCHighlighter)
};

#endif  // OBJECTIVECHIGHLIGHTER_H
