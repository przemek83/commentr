#ifndef JAVASCRIPTHIGHLIGHTER_H
#define JAVASCRIPTHIGHLIGHTER_H

#include "Highlighter.h"

class JavaScriptHighlighter : public Highlighter
{
public:
    explicit JavaScriptHighlighter(QObject* parent);
    ~JavaScriptHighlighter() override = default;

protected:
    void initRules() override;

    void commentBlock(const QString& text) override;

private:
    Q_DISABLE_COPY_MOVE(JavaScriptHighlighter)

    HighlightingRule singleLineCommentRule_;

    HighlightingRule multiLineCommentRule_;
};

#endif  // JAVASCRIPTHIGHLIGHTER_H
