#include <QBuffer>
#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include <QKeyEvent>

#include "File.h"
#include "FtpFileSaver.h"
#include "Config.h"

FtpFileSaver::FtpFileSaver(QObject *parent) :
    QObject(parent), fileToSave_(NULL)
{
    connect(&ftp_, SIGNAL(commandFinished(int,bool)),
            this, SLOT(ftpCommandFinished(int,bool)));
    connect(&ftp_, SIGNAL(dataTransferProgress(qint64,qint64)),
            this, SLOT(updateDataTransferProgress(qint64,qint64)));

    progressDialog_.setVisible(false);
    progressDialog_.setModal(true);
    progressDialog_.setRange(0, 0);
    progressDialog_.setCancelButton(NULL);

    ftp_.setTransferMode(QFtp::Passive);
}

FtpFileSaver::~FtpFileSaver()
{
    if( NULL != fileToSave_ )
    {
        delete fileToSave_;
    }
}

void FtpFileSaver::saveFile(File* file)
{
    fileToSave_ = file;
    ftp_.connectToHost(Config::getInstance().ftpHost());
    ftp_.login(Config::getInstance().ftpLogin(),
               Config::getInstance().ftpPassword());
    ftp_.cd(fileToSave_->path());

    ///Create QByteArray and init buffer for file transfer.
    QByteArray byteBuffer;
    byteBuffer.clear();
    byteBuffer.append(*fileToSave_->content());
    buffer_.setData(byteBuffer);
    buffer_.open(QIODevice::ReadOnly);
    ftp_.put(&buffer_, fileToSave_->getName());
}

void FtpFileSaver::checkConnection(QString host,
                                   QString login,
                                   QString password)
{
    ftp_.connectToHost(host);
    ftp_.login(login, password);
}

void FtpFileSaver::finish(bool success, QString error)
{
    if( false == success )
    {
        QMessageBox::information(Common::getMainWindow(this), tr("FTP"), error);
    }
    ftp_.clearPendingCommands();
    ftp_.close();
    emit operationFinished(success);
}

void FtpFileSaver::windowResized()
{
    Common::centerWidget(this, &progressDialog_);
}

void FtpFileSaver::ftpCommandFinished(int, bool error)
{
    switch( ftp_.currentCommand() )
    {
        case QFtp::ConnectToHost:
        {
            if( true == error )
            {
                finish(false, tr("Unable to connect to the FTP server.\n"
                                 "Error: %1").arg(ftp_.errorString()));
                return;
            }

            break;
        }

        case QFtp::Login:
        {
            if( true == error )
            {
                finish(false, tr("Unable to login. Wrong user or password.\n"
                                 "Error: %1").arg(ftp_.errorString()));
                return;
            }

            if( NULL == fileToSave_ )
            {
                emit operationFinished(true);
                return;
            }

            break;
        }

        case QFtp::Cd:
        {
            if( true == error )
            {
                finish(false, tr("Unable to change directory.\n"
                                 "Error: %1").arg(ftp_.errorString()));
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
}

void FtpFileSaver::updateDataTransferProgress(qint64 readBytes, qint64 totalBytes)
{
    if( false == progressDialog_.isVisible() )
    {
        Common::centerWidget(this, &progressDialog_);
        progressDialog_.setVisible(true);
    }
    progressDialog_.setMaximum(totalBytes);
    progressDialog_.setValue(readBytes);
}
