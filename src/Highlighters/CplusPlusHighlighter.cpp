#include "CplusPlusHighlighter.h"

CplusPlusHighlighter::CplusPlusHighlighter(const SpellChecker& spellChecker,
                                           QObject* parent)
    : CHighlighter(spellChecker, parent)
{
}

QStringList CplusPlusHighlighter::getKeywords() const { return keywords_; }
