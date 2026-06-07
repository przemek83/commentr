#include "ObjectiveCHighlighter.h"

ObjectiveCHighlighter::ObjectiveCHighlighter(const SpellChecker& spellChecker)
    : CFamilyHighlighter(spellChecker, "objectiveC.txt")
{
}

void ObjectiveCHighlighter::initRules()
{
    CFamilyHighlighter::initRules();
    addRule(classPattern_, SyntaxElement::CLASS);
}
