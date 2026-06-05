#ifndef HIGHLIGHTERTESTHELPERS_H
#define HIGHLIGHTERTESTHELPERS_H

class QTextBlock;
class QTextCharFormat;
class QString;

bool hasFormatForText(const QTextBlock& block, const QString& text,
                      const QTextCharFormat& expectedFormat,
                      bool italic = false);

#endif  // HIGHLIGHTERTESTHELPERS_H
