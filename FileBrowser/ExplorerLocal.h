#ifndef EXPLORERLOCAL_H
#define EXPLORERLOCAL_H

#include <QListView>

#include "Explorer.h"

class File;

class ExplorerLocal : public QListView, public Explorer
{
    Q_OBJECT
public:
    ExplorerLocal(bool open, QWidget *parent = 0);

    virtual ~ExplorerLocal();

    virtual void setPath(QString path);

    virtual bool fileIsValid(QString file);

    virtual bool isWrapping();

    virtual void setWrapping(bool wrapping);

    virtual QString getCurrentPath();

    virtual void initialize();

    virtual bool initialized();

    virtual void performOperationOnFile(QString filePath);

protected:
    virtual QListView* getListView();

    /**
     * @brief update path lineEdit if file was clicked.
     * @param index idex clicked.
     */
    virtual void listViewItemClicked(const QModelIndex &index);

    virtual void mouseMoveEvent(QMouseEvent* event);

protected slots:
    virtual void itemActivated(QModelIndex index);

private:
    Q_DISABLE_COPY(ExplorerLocal)

    void initModelAndView();

private slots:
    void directoryLoaded(const QString& path);

    bool directoryIsAccessible(const QString& path);

signals:
    void pathChanged(QString newPath);

    void filePrepared(File* file);
};

#endif // EXPLORERLOCAL_H
