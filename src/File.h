#ifndef FILE_H
#define FILE_H

#include <QObject>
#include <QString>

#include "Common.h"

class File
{
public:
    File(Common::Source source, QString path, QString name, QString suffix,
         QString content);
    File(File&& other) = default;

    virtual ~File() = default;

    File& operator=(File&& other) = default;

    Common::Source source() const;
    void setSource(const Common::Source& source);

    QString path() const;
    void setPath(const QString& path);

    QString baseName() const;
    void setBaseName(const QString& baseName);

    QString suffix() const;
    void setSuffix(const QString& suffix);

    const QString& content() const;

    void clearContent();

    QString getFilePath() const;

    static QString filePathToPath(const QString& filePath);

    static QString filePathToFileName(const QString& filePath);

    static QString filePathToBaseName(const QString& filePath);

    static QString filePathToSuffix(const QString& filePath);

    static QString fileNameToBaseName(const QString& fileName);

    static QString fileNameToSuffix(const QString& fileName);

private:
    Q_DISABLE_COPY(File)

    Common::Source source_;

    QString path_;

    QString baseName_;

    QString suffix_;

    QString content_;
};

#endif  // FILE_H
