#ifndef CPLUSPLUSHIGHLIGHTER_H
#define CPLUSPLUSHIGHLIGHTER_H

#include "CFamilyHighlighter.h"

class CplusPlusHighlighter : public CFamilyHighlighter
{
public:
    explicit CplusPlusHighlighter(QObject* parent = nullptr);

protected:
    void initRules() override;

private:
    Q_DISABLE_COPY(CplusPlusHighlighter)
};

#endif  // CPLUSPLUSHIGHLIGHTER_H
