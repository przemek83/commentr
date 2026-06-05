#include "HighlighterTestHelpers.h"

#include <QString>
#include <QTextBlock>
#include <QTextCharFormat>
#include <QTextLayout>

bool hasFormatForText(const QTextBlock& block, const QString& text,
                      const QTextCharFormat& expectedFormat, bool italic)
{
    const qsizetype position{block.text().indexOf(text)};
    if (position < 0)
        return false;

    QTextLayout* layout{block.layout()};
    if (layout == nullptr)
        return false;

    for (const QTextLayout::FormatRange& range : layout->formats())
    {
        if ((range.start <= position) &&
            (position + text.length() <= range.start + range.length))
        {
            const QTextCharFormat& format{range.format};
            if ((format.foreground().color() ==
                 expectedFormat.foreground().color()) &&
                ((expectedFormat.fontWeight() < 0) ||
                 (format.fontWeight() == expectedFormat.fontWeight())) &&
                ((!italic) || format.fontItalic()))
            {
                return true;
            }
        }
    }

    return false;
}