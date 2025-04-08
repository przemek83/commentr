#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QTextStream>
#include <QWidget>

#include "Common.h"

const int Common::timerFireInterval_ = 200;

const int Common::maxRecentFiles_ = 7;

const QString Common::rootPath()
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

    if (false == file.open(QIODevice::ReadOnly))
    {
        return "";
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    return stream.readAll();
}

QString Common::saveFile(QString fileName, QString& data)
{
    QFile file(fileName);

    if (false == file.open(QIODevice::WriteOnly | QIODevice::Truncate))
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

int Common::timerFireInterval() { return timerFireInterval_; }

QWidget* Common::getMainWindow(QObject* startObject)
{
    QObject* object = startObject;
    while (object->parent() != nullptr)
    {
        object = object->parent();
    }

    return static_cast<QWidget*>(object);
}

void Common::centerWidget(QObject* hierarchyObject, QWidget* widgetToCenter)
{
    static const QWidget* mainWindow = getMainWindow(hierarchyObject);
    QPoint applicationCenter = mainWindow->frameGeometry().center();
    widgetToCenter->move(applicationCenter - widgetToCenter->rect().center());
}

int Common::getMaxRecentFiles() { return maxRecentFiles_; }

float Common::normalizeFont(float fontSize)
{
    float normalizedFonSize = fontSize;

    if (fontSize > 128)
    {
        normalizedFonSize = 128;
    }

    if (fontSize < 1)
    {
        normalizedFonSize = 1;
    }

    return normalizedFonSize;
}
