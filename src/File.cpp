#include <QDebug>

#include "File.h"

File::File(Common::Source source, QString path, QString name, QString suffix,
           QString* content)
    : source_(source),
      path_(path),
      baseName_(name),
      suffix_(suffix),
      content_(content)
{
}

File::~File()
{
    if (content_ != nullptr)
    {
        delete content_;
    }
}

Common::Source File::source() const { return source_; }

void File::setSource(const Common::Source& source) { source_ = source; }

QString File::path() const { return path_; }

void File::setPath(const QString& path) { path_ = path; }

QString File::baseName() const { return baseName_; }

void File::setBaseName(const QString& baseName) { baseName_ = baseName; }

QString File::suffix() const { return suffix_; }

void File::setSuffix(const QString& suffix) { suffix_ = suffix; }

QString* File::content() const { return content_; }

void File::setContent(QString* content) { content_ = content; }

void File::clearContent()
{
    delete content_;
    content_ = nullptr;
}

QString File::getFilePath() const
{
    QString filePath(path());
    filePath += '/';
    filePath += baseName();
    if (false == suffix().isEmpty())
    {
        filePath += '.';
        filePath += suffix();
    }
    return filePath;
}

QString File::getName()
{
    QString fileName;
    fileName += baseName();
    if (false == suffix().isEmpty())
    {
        fileName += '.';
        fileName += suffix();
    }

    return fileName;
}

QString File::dump()
{
    QString dumpString;
    dumpString.append("Path: " + path_ + " base name: " + baseName_ +
                      " suffix: " + suffix_ + " filepath " + getFilePath());

    return dumpString;
}

QString File::filePathToPath(QString filePath)
{
    return filePath.left(filePath.lastIndexOf('/'));
}

QString File::filePathToFileName(QString filePath)
{
    return filePath.right(filePath.size() - filePath.lastIndexOf('/') - 1);
}

QString File::filePathToBaseName(QString filePath)
{
    return fileNameToBaseName(filePathToFileName(filePath));
}

QString File::filePathToSuffix(QString filePath)
{
    return fileNameToSuffix(filePathToFileName(filePath));
}

QString File::fileNameToBaseName(QString fileName)
{
    int lastDotIndex = fileName.lastIndexOf('.');
    // In case when file start with '.'.
    if (lastDotIndex == 0)
    {
        lastDotIndex = -1;
    }
    QString baseName = fileName;

    // If found.
    if (-1 != lastDotIndex)
    {
        baseName.truncate(lastDotIndex);
    }

    return baseName;
}

QString File::fileNameToSuffix(QString fileName)
{
    int lastDotIndex = fileName.lastIndexOf('.');
    // In case when file start with '.'.
    if (lastDotIndex == 0)
    {
        lastDotIndex = -1;
    }
    QString suffix("");

    // If found.
    if (-1 != lastDotIndex)
    {
        suffix = fileName;
        suffix.remove(0, lastDotIndex + 1);
    }

    return suffix;
}
