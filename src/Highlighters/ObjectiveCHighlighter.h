#ifndef OBJECTIVECHIGHLIGHTER_H
#define OBJECTIVECHIGHLIGHTER_H

#include "CFamilyHighlighter.h"

class ObjectiveCHighlighter : public CFamilyHighlighter
{
public:    
    explicit ObjectiveCHighlighter(QObject *parent = 0);

    virtual ~ObjectiveCHighlighter();

protected:
    virtual void initRules();

private:
    Q_DISABLE_COPY(ObjectiveCHighlighter)
};

#endif // OBJECTIVECHIGHLIGHTER_H
