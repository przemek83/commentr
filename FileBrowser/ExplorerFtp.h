#ifndef EXPLORERFTP_H
#define EXPLORERFTP_H

#include <QListWidget>
#include <QListWidgetItem>
#include <QBuffer>
#include <QProgressDialog>
#include "qftp.h"

#include "Explorer.h"

class File;

class ExplorerFtp : public QListWidget, public Explorer
{
    Q_OBJECT
public:
    ExplorerFtp(bool open, QWidget* parent = 0);

    virtual ~ExplorerFtp();

    virtual void setPath(QString path);

    virtual QString getCurrentPath();

    ///Check if given file can be used as in/out file.
    virtual bool fileIsValid(QString file);

    virtual bool isWrapping();

    virtual void setWrapping(bool wrapping);

    virtual void initialize();

    virtual bool initialized();

    virtual void performOperationOnFile(QString filePath);

protected:
    virtual void resizeEvent(QResizeEvent* event);

    virtual QListView* getListView();

    /**
     * @brief update path lineEdit if file was clicked.
     * @param index idex clicked.
     */
    virtual void listViewItemClicked(const QModelIndex &index);

protected slots:
    void updateDataTransferProgress(qint64 readBytes, qint64 totalBytes);

    void ftpCommandFinished(int, bool error);

    virtual void itemActivated(QModelIndex index);

private:
    Q_DISABLE_COPY(ExplorerFtp)

    QFtp* ftp_;

    class Item : public QListWidgetItem
    {
    public:
        Item(const QString& text, bool dir, bool readable, bool writeable);

        virtual ~Item();

        virtual bool operator<(const QListWidgetItem& other) const;

        bool isDir() const;

        bool readable() const;

        bool writeable() const;

    private:
        bool readable_;

        bool writeable_;
    };

    QString getPathToUse(Item* itemClicked);

    //Current path of browser.
    QString currentPath_;

    //New path used by cd command.
    QString newPath_;

    QList<Item*> itemsList_;

    QBuffer fileBuffer_;

    QProgressDialog progressDialog_;

private slots:
    void addToList(const QUrlInfo& urlInfo);

    void cancelDownload();

signals:
    void filePrepared(File* file);

    void pathChanged(QString newPath);
};

#endif // EXPLORERFTP_H
