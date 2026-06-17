#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QListView>

class File;
class Config;
class QFileSystemModel;

class FileExplorer : public QListView
{
    Q_OBJECT
public:
    FileExplorer(bool open, Config& config, QWidget* parent);

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
    void itemWasActivated(QModelIndex index);

private:
    Q_DISABLE_COPY_MOVE(FileExplorer)

    void setupList();

    void changeRootIndex(const QFileSystemModel& model, const QString& path);

    QString currentItem_;

    Config& config_;

    bool open_;

private slots:
    void directoryLoaded(const QString& path);

    bool directoryIsAccessible(const QString& path);

signals:
    void pathChanged(QString newPath);

    void filePrepared(File file);
};

#endif  // FILEEXPLORER_H
