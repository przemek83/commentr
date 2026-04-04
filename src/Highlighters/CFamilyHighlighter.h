#ifndef CFAMILYHIGHLIGHTER_H
#define CFAMILYHIGHLIGHTER_H

#include <QRegularExpression>

#include "Highlighter.h"

class CFamilyHighlighter : public Highlighter
{
public:
    explicit CFamilyHighlighter(QObject* parent);
    ~CFamilyHighlighter() override = default;

protected:
    void commentBlock(const QString& text) override;

    void initFunctionsRules();

    void initQuotationRules();

private:
    Q_DISABLE_COPY_MOVE(CFamilyHighlighter)

    HighlightingRule singleLineCommentRule_;

    HighlightingRule multiLineCommentRule_;
};

#endif  // CFAMILYHIGHLIGHTER_H
