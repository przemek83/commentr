#ifndef CSHARPHIGHLIGHTER_H
#define CSHARPHIGHLIGHTER_H

#include "CFamilyHighlighter.h"

class CSharpHighlighter : public CFamilyHighlighter
{
public:
    explicit CSharpHighlighter(QObject* parent = 0);

protected:
    void initRules() override;

private:
    Q_DISABLE_COPY(CSharpHighlighter)
};

#endif  // CSHARPHIGHLIGHTER_H
