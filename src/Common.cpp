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

QString Common::loadFile(const QString& name)
{
    QFile file(name);

    if (!file.open(QIODevice::ReadOnly))
        return QLatin1String("");

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);

    return stream.readAll();
}

QString Common::saveFile(const QString& fileName, const QString& data)
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
    static const QString stylesheet =
        Common::loadFile(QStringLiteral(":/style.css"));
    return stylesheet;
}

int Common::timerFireInterval()
{
    const int timerFireInterval{200};
    return timerFireInterval;
}

QTextCharFormat Common::getFormat(SyntaxElement element)
{
    QTextCharFormat format;

    switch (element)
    {
        case SyntaxElement::KEYWORD:
            format.setForeground(Qt::darkBlue);
            format.setFontWeight(QFont::Bold);
            break;

        case SyntaxElement::QUOTATION:
            format.setForeground(Qt::darkGreen);
            break;

        case SyntaxElement::COMMENT:
            format.setForeground(Qt::red);
            break;

        case SyntaxElement::FUNCTION:
            format.setFontItalic(true);
            format.setForeground(Qt::blue);
            break;

        case SyntaxElement::CLASS:
            format.setForeground(Qt::darkMagenta);
            format.setFontWeight(QFont::Bold);
            break;

        case SyntaxElement::MISSPELLED_WORD:
            format.setForeground(Qt::red);
            format.setUnderlineColor(QColor(Qt::red));
            format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
            break;

        default:
            break;
    }

    return format;
}

QWidget* Common::getMainWindow(QObject* startObject)
{
    QObject* object{startObject};
    while (object->parent() != nullptr)
        object = object->parent();

    return dynamic_cast<QWidget*>(object);
}

int Common::getMaxRecentFiles()
{
    const int maxRecentFiles{7};
    return maxRecentFiles;
}

float Common::normalizeFont(float fontSize)
{
    float normalizedFonSize{fontSize};

    if (const int maxFontSize{128}; fontSize > maxFontSize)
        normalizedFonSize = maxFontSize;

    if (const int minFontSize{1}; fontSize < minFontSize)
        normalizedFonSize = minFontSize;

    return normalizedFonSize;
}
