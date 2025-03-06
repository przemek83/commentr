#ifndef CPLUSPLUSHIGHLIGHTER_H
#define CPLUSPLUSHIGHLIGHTER_H

#include "CFamilyHighlighter.h"

class CplusPlusHighlighter : public CFamilyHighlighter
{
public:
    explicit CplusPlusHighlighter(QObject* parent = 0);

    virtual ~CplusPlusHighlighter();

protected:
    virtual void initRules();

private:
    Q_DISABLE_COPY(CplusPlusHighlighter)
};

#endif // CPLUSPLUSHIGHLIGHTER_H
