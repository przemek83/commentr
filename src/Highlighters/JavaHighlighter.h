#ifndef JAVAHIGHLIGHTER_H
#define JAVAHIGHLIGHTER_H

#include "CFamilyHighlighter.h"

class JavaHighlighter : public CFamilyHighlighter
{
public:
    JavaHighlighter(const SpellChecker& spellChecker, QObject* parent);
    ~JavaHighlighter() override = default;

protected:
    void initRules() override;

private:
    Q_DISABLE_COPY_MOVE(JavaHighlighter)

    const QStringList keywords_{loadKeywords("java.txt")};
};

#endif  // JAVAHIGHLIGHTER_H
