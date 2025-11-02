#ifndef EXPLORERLOCAL_H
#define EXPLORERLOCAL_H

#include <QListView>

#include "Explorer.h"

class File;

class ExplorerLocal : public QListView, public Explorer
{
    Q_OBJECT
public:
    ExplorerLocal(bool open, QWidget* parent);

    ~ExplorerLocal() override;

    void setPath(QString path) override;

    bool fileIsValid(QString file) override;

    bool isWrapping() override;

    void setWrapping(bool wrapping) override;

    QString getCurrentPath() override;

    void initialize() override;

    bool initialized() override;

    void performOperationOnFile(QString filePath) override;

protected:
    QListView* getListView() override;

    void listViewItemClicked(const QModelIndex& index) override;

    void mouseMoveEvent(QMouseEvent* event) override;

protected slots:
    void itemActivated(QModelIndex index) override;

private:
    Q_DISABLE_COPY(ExplorerLocal)

    void initModelAndView();

    QString currentItem_;

private slots:
    void directoryLoaded(const QString& path);

    bool directoryIsAccessible(const QString& path);

signals:
    void pathChanged(QString newPath);

    void filePrepared(File* file);
};

#endif  // EXPLORERLOCAL_H
