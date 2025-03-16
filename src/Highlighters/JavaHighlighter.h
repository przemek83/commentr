#ifndef JAVAHIGHLIGHTER_H
#define JAVAHIGHLIGHTER_H

#include "CFamilyHighlighter.h"

class JavaHighlighter : public CFamilyHighlighter
{
public:
    explicit JavaHighlighter(QObject* parent = 0);

protected:
    virtual void initRules();

private:
    Q_DISABLE_COPY(JavaHighlighter)
};

#endif  // JAVAHIGHLIGHTER_H
