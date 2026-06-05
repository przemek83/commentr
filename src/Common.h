#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QTextCharFormat>

#include "SyntaxElement.h"

class QWidget;
class QObject;

class Common
{
public:
    static QString loadFile(QString name);

    static QString saveFile(QString fileName, const QString& data);

    static QString getStyleSheet();

    static int timerFireInterval();

    static QString rootPath();

    enum class Source
    {
        LOCAL = 0,
        NOT_SET
    };

    static QTextCharFormat getFormat(SyntaxElement element);

    static QWidget* getMainWindow(QObject* startObject);

    static int getMaxRecentFiles();

    static float normalizeFont(float fontSize);

private:
    Common() = default;
};

#endif  // COMMON_H
