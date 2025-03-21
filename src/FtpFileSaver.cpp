#include <QApplication>
#include <QBuffer>
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>

#include "File.h"
#include "FtpFileSaver.h"

FtpFileSaver::FtpFileSaver(QObject* parent)
    : QObject(parent), fileToSave_(nullptr)
{
#ifdef FTP
    connect(&ftp_, SIGNAL(commandFinished(int, bool)), this,
            SLOT(ftpCommandFinished(int, bool)));
    connect(&ftp_, SIGNAL(dataTransferProgress(qint64, qint64)), this,
            SLOT(updateDataTransferProgress(qint64, qint64)));
    ftp_.setTransferMode(QFtp::Passive);
#endif

    progressDialog_.setVisible(false);
    progressDialog_.setModal(true);
    progressDialog_.setRange(0, 0);
    progressDialog_.setCancelButton(nullptr);
}

FtpFileSaver::~FtpFileSaver()
{
    if (fileToSave_ != nullptr)
    {
        delete fileToSave_;
    }
}

void FtpFileSaver::saveFile(File* file)
{
    fileToSave_ = file;

#ifdef FTP
    ftp_.connectToHost(Config::getInstance().ftpHost());
    ftp_.login(Config::getInstance().ftpLogin(),
               Config::getInstance().ftpPassword());
    ftp_.cd(fileToSave_->path());
#endif

    /// Create QByteArray and init buffer for file transfer.
    QByteArray byteBuffer;
    byteBuffer.clear();
    byteBuffer.append(fileToSave_->content()->toStdString());
    buffer_.setData(byteBuffer);
    buffer_.open(QIODevice::ReadOnly);

#ifdef FTP
    ftp_.put(&buffer_, fileToSave_->getName());
#endif
}

void FtpFileSaver::checkConnection([[maybe_unused]] QString host,
                                   [[maybe_unused]] QString login,
                                   [[maybe_unused]] QString password)
{
#ifdef FTP
    ftp_.connectToHost(host);
    ftp_.login(login, password);
#endif
}

void FtpFileSaver::finish(bool success, QString error)
{
    if (false == success)
    {
        QMessageBox::information(Common::getMainWindow(this), tr("FTP"), error);
    }

#ifdef FTP
    ftp_.clearPendingCommands();
    ftp_.close();
#endif
    emit operationFinished(success);
}

void FtpFileSaver::windowResized()
{
    Common::centerWidget(this, &progressDialog_);
}

void FtpFileSaver::ftpCommandFinished(int, [[maybe_unused]] bool error)
{
#ifdef FTP
    switch (ftp_.currentCommand())
    {
        case QFtp::ConnectToHost:
        {
            if (true == error)
            {
                finish(false, tr("Unable to connect to the FTP server.\n"
                                 "Error: %1")
                                  .arg(ftp_.errorString()));
                return;
            }

            break;
        }

        case QFtp::Login:
        {
            if (true == error)
            {
                finish(false, tr("Unable to login. Wrong user or password.\n"
                                 "Error: %1")
                                  .arg(ftp_.errorString()));
                return;
            }

            if (fileToSave_ == nullptr)
            {
                emit operationFinished(true);
                return;
            }

            break;
        }

        case QFtp::Cd:
        {
            if (true == error)
            {
                finish(false, tr("Unable to change directory.\n"
                                 "Error: %1")
                                  .arg(ftp_.errorString()));
                return;
            }
            break;
        }

        case QFtp::Put:
        {
            finish(true, "");
            break;
        }

        default:
        {
            break;
        }
    }
#endif
}

void FtpFileSaver::updateDataTransferProgress(qint64 readBytes,
                                              qint64 totalBytes)
{
    if (false == progressDialog_.isVisible())
    {
        Common::centerWidget(this, &progressDialog_);
        progressDialog_.setVisible(true);
    }
    progressDialog_.setMaximum(totalBytes);
    progressDialog_.setValue(readBytes);
}
