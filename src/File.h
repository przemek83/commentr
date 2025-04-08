#ifndef FILE_H
#define FILE_H

#include <QObject>
#include <QString>

#include "Common.h"

class File
{
public:
    File(Common::Source source, QString path, QString name, QString suffix,
         QString* content);

    virtual ~File();

    Common::Source source() const;
    void setSource(const Common::Source& source);

    QString path() const;
    void setPath(const QString& path);

    QString baseName() const;
    void setBaseName(const QString& baseName);

    QString suffix() const;
    void setSuffix(const QString& suffix);

    QString* content() const;
    void setContent(QString* content);

    void clearContent();

    /**
     * @brief create from path, base name and suffix complete filepath.
     * @return filepath.
     */
    QString getFilePath() const;

    /**
     * @brief create from base name and suffix name of file.
     * @return file name.
     */
    QString getName() const;

    QString dump();

    static QString filePathToPath(QString filePath);

    static QString filePathToFileName(QString filePath);

    static QString filePathToBaseName(QString filePath);

    static QString filePathToSuffix(QString filePath);

    static QString fileNameToBaseName(QString fileName);

    static QString fileNameToSuffix(QString fileName);

private:
    Q_DISABLE_COPY(File)

    Common::Source source_;

    QString path_;

    QString baseName_;

    QString suffix_;

    QString* content_;
};

#endif  // FILE_H
