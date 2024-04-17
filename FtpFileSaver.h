#ifndef FTPFILESAVER_H
#define FTPFILESAVER_H

#include <QBuffer>
#include <QObject>
#include <QProgressDialog>
#include "qftp.h"

class File;

class FtpFileSaver : public QObject
{
    Q_OBJECT
public:
    explicit FtpFileSaver(QObject *parent = 0);

    virtual ~FtpFileSaver();

    void saveFile(File* file);

    void checkConnection(QString host,
                         QString login,
                         QString password);

protected slots:
    void ftpCommandFinished(int, bool error);

    void updateDataTransferProgress(qint64 readBytes, qint64 totalBytes);

private:
    Q_DISABLE_COPY(FtpFileSaver)

    void finish(bool success, QString error);

    QProgressDialog progressDialog_;

    ///QFtp object.
    QFtp ftp_;

    ///pointer to file which need to be saved.
    File* fileToSave_;

    ///Buffer used for file transfer.
    QBuffer buffer_;

private slots:
    void windowResized();

signals:
    void operationFinished(bool success);
};

#endif // FTPFILESAVER_H
