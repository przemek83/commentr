#ifndef FTPFILESAVER_H
#define FTPFILESAVER_H

#include <QBuffer>
#include <QObject>
#include <QProgressDialog>

#ifdef FTP
#include "qftp.h"
#endif

class File;

class FtpFileSaver : public QObject
{
    Q_OBJECT
public:
    explicit FtpFileSaver(QObject* parent = 0);

    ~FtpFileSaver() override;

    void saveFile(File* file);

    void checkConnection(QString host, QString login, QString password);

protected slots:
    void ftpCommandFinished(int, bool error);

    void updateDataTransferProgress(qint64 readBytes, qint64 totalBytes);

private:
    Q_DISABLE_COPY(FtpFileSaver)

    void finish(bool success, QString error);

    QProgressDialog progressDialog_;

#ifdef FTP
    QFtp ftp_;
#endif

    File* fileToSave_;

    QBuffer buffer_;

private slots:
    void windowResized();

signals:
    void operationFinished(bool success);
};

#endif  // FTPFILESAVER_H
