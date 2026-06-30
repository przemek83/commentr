#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QListView>

#include "FileAccessMode.h"

class File;
class Config;
class QFileSystemModel;

class FileExplorer : public QListView
{
    Q_OBJECT
public:
    FileExplorer(FileAccessMode mode, Config& config, QWidget* parent);

    ~FileExplorer() override;

    void setPath(const QString& path);

    bool fileIsValid(const QString& file) const;

    bool isWrappingContent() const;

    QString getCurrentPath() const;

    void initialize();

    bool isInitialized() const;

    void performOperationOnFile(const QString& filePath);

protected:
    void mouseMoveEvent(QMouseEvent* e) override;

private slots:
    void itemWasActivated(const QModelIndex& index);

private:
    Q_DISABLE_COPY_MOVE(FileExplorer)

    void setupList();

    void changeRootIndex(const QFileSystemModel& model, const QString& path);

    bool doesUserWantsToOverwriteFile(const QString& filePath);

    QString currentItem_;

    Config& config_;

    FileAccessMode mode_;

private slots:
    void directoryLoaded(const QString& path);

    bool directoryIsAccessible(const QString& path);

signals:
    void pathChanged(QString newPath);

    void filePrepared(File file);
};

#endif  // FILEEXPLORER_H
