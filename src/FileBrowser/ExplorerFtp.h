#ifndef EXPLORERFTP_H
#define EXPLORERFTP_H

#include <QBuffer>
#include <QListWidget>
#include <QListWidgetItem>
#include <QProgressDialog>

#include "Explorer.h"

class File;
class QFtp;
class QUrlInfo;

class ExplorerFtp : public QListWidget, public Explorer
{
    Q_OBJECT
public:
    ExplorerFtp(bool open, QWidget* parent);

    ~ExplorerFtp() override;

    void setPath(QString path) override;

    QString getCurrentPath() override;

    /// Check if given file can be used as in/out file.
    bool fileIsValid(QString file) override;

    bool isWrapping() override;

    void setWrapping(bool wrapping) override;

    void initialize() override;

    bool initialized() override;

    void performOperationOnFile(QString filePath) override;

protected:
    void resizeEvent(QResizeEvent* event) override;

    QListView* getListView() override;

    /**
     * @brief update path lineEdit if file was clicked.
     * @param index idex clicked.
     */
    void listViewItemClicked(const QModelIndex& index) override;

protected slots:
    void updateDataTransferProgress(qint64 readBytes, qint64 totalBytes);

    void ftpCommandFinished(int, bool error);

    void itemActivated(QModelIndex index) override;

private:
    Q_DISABLE_COPY(ExplorerFtp)

    QFtp* ftp_{nullptr};

    class Item : public QListWidgetItem
    {
    public:
        Item(const QString& text, bool dir, bool readable, bool writeable);

        bool operator<(const QListWidgetItem& other) const override;

        bool isDir() const;

        bool readable() const;

        bool writeable() const;

    private:
        bool readable_;

        bool writeable_;
    };

    QString getPathToUse(Item* itemClicked);

    // Current path of browser.
    QString currentPath_;

    // New path used by cd command.
    QString newPath_;

    QList<Item*> itemsList_;

    QBuffer fileBuffer_;

    QProgressDialog progressDialog_;

private slots:
#ifdef FTP
    void addToList(const QUrlInfo& urlInfo);
#endif

    void cancelDownload();

signals:
    void filePrepared(File* file);

    void pathChanged(QString newPath);
};

#endif  // EXPLORERFTP_H
