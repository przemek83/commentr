#ifndef CPLUSPLUSHIGHLIGHTER_H
#define CPLUSPLUSHIGHLIGHTER_H

#include "CFamilyHighlighter.h"

class CplusPlusHighlighter : public CFamilyHighlighter
{
public:
    explicit CplusPlusHighlighter(QObject* parent);
    ~CplusPlusHighlighter() override = default;

protected:
    void initRules() override;

private:
    Q_DISABLE_COPY_MOVE(CplusPlusHighlighter)
};

#endif  // CPLUSPLUSHIGHLIGHTER_H
