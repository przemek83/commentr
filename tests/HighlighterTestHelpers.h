#ifndef HIGHLIGHTERTESTHELPERS_H
#define HIGHLIGHTERTESTHELPERS_H

#include <QString>
#include <QTest>
#include <QTextBlock>
#include <QTextCharFormat>
#include <QTextDocument>

#include "Highlighters/Highlighter.h"

bool hasFormatForText(const QTextBlock& block, const QString& text,
                      const QTextCharFormat& expectedFormat,
                      bool italic = false);

QTextBlock setupHighlighter(Highlighter& highlighter, QTextDocument& document,
                            const QString& source);

#endif  // HIGHLIGHTERTESTHELPERS_H
