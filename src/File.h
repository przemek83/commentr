#ifndef FILE_H
#define FILE_H

#include <QObject>
#include <QString>

#include "Common.h"

class File
{
public:
    File(Common::Source source, QString filePath, QString content);
    File(File&& other) = default;
    File(const File& other) = default;

    virtual ~File() = default;

    File& operator=(File&& other) = default;
    File& operator=(const File& other) = default;

    Common::Source source() const;
    void setSource(const Common::Source& source);

    QString getFilePath() const;
    void setFilePath(const QString& filePath);

    QString path() const;

    QString baseName() const;

    QString suffix() const;

    const QString& content() const;

    void clearContent();

private:
    Common::Source source_;

    QString filePath_;

    QString content_;
};

#endif  // FILE_H
