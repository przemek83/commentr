#include "Common.h"

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QTextStream>
#include <QWidget>

QString Common::rootPath()
{
    const QStringList potentialPaths{QStandardPaths::standardLocations(
        QStandardPaths::AppLocalDataLocation)};
    for (const auto& path : potentialPaths)
    {
        const QFileInfo currentPath(path);
        if (currentPath.isDir() && currentPath.isWritable())
            return path;
    }
    return potentialPaths.constFirst();
}

QString Common::loadFile(QString name)
{
    QFile file(name);

    if (!file.open(QIODevice::ReadOnly))
        return "";

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);

    return stream.readAll();
}

QString Common::saveFile(QString fileName, QString& data)
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QString error(QObject::tr("Saving file failed. Can't open: "));
        error.append(fileName);
        error.append(" for writing.");
        return error;
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    stream << data;

    file.close();
    return QObject::tr("File saved...");
}

QString Common::getStyleSheet()
{
    static const QString stylesheet = Common::loadFile(":/style.css");
    return stylesheet;
}

int Common::timerFireInterval()
{
    const int timerFireInterval{200};
    return timerFireInterval;
}

QWidget* Common::getMainWindow(QObject* startObject)
{
    QObject* object = startObject;
    while (object->parent() != nullptr)
        object = object->parent();

    return static_cast<QWidget*>(object);
}

void Common::centerWidget(QObject* hierarchyObject, QWidget* widgetToCenter)
{
    const QWidget* mainWindow{getMainWindow(hierarchyObject)};
    QPoint applicationCenter{mainWindow->frameGeometry().center()};
    widgetToCenter->move(applicationCenter - widgetToCenter->rect().center());
}

int Common::getMaxRecentFiles()
{
    const int maxRecentFiles{7};
    return maxRecentFiles;
}

float Common::normalizeFont(float fontSize)
{
    float normalizedFonSize{fontSize};

    const int maxFontSize{128};
    if (fontSize > maxFontSize)
        normalizedFonSize = maxFontSize;

    const int minFontSize{1};
    if (fontSize < minFontSize)
        normalizedFonSize = minFontSize;

    return normalizedFonSize;
}
