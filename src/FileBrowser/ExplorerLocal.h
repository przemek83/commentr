#ifndef EXPLORERLOCAL_H
#define EXPLORERLOCAL_H

#include <qtclasshelpermacros.h>
#include <QListView>

#include "Explorer.h"

class File;
class Config;

class ExplorerLocal : public QListView, public Explorer
{
    Q_OBJECT
public:
    ExplorerLocal(bool open, Config& config, QWidget* parent);

    ~ExplorerLocal() override;

    void setPath(QString path) override;

    bool fileIsValid(QString file) override;

    bool isWrappingContent() override;

    QString getCurrentPath() override;

    void initialize() override;

    bool initialized() override;

    void performOperationOnFile(QString filePath) override;

protected:
    QListView* getListView() override;

    void listViewItemClicked(const QModelIndex& index) override;

    void mouseMoveEvent(QMouseEvent* event) override;

private slots:
    void itemWasActivated(QModelIndex index);

private:
    Q_DISABLE_COPY_MOVE(ExplorerLocal)

    void setupList();

    QString currentItem_;

    Config& config_;

private slots:
    void directoryLoaded(const QString& path);

    bool directoryIsAccessible(const QString& path);

signals:
    void pathChanged(QString newPath);

    void filePrepared(File* file);
};

#endif  // EXPLORERLOCAL_H
