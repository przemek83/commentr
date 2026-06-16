#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QListView>

class File;
class Config;

class FileExplorer : public QListView
{
    Q_OBJECT
public:
    FileExplorer(bool open, Config& config, QWidget* parent);

    ~FileExplorer() override;

    void setPath(QString path);

    bool fileIsValid(QString file);

    bool isWrappingContent();

    QString getCurrentPath();

    void initialize();

    bool initialized();

    void performOperationOnFile(QString filePath);

protected:
    QListView* getListView();

    void listViewItemClicked(const QModelIndex& index);

    void mouseMoveEvent(QMouseEvent* e) override;

private slots:
    void itemWasActivated(QModelIndex index);

private:
    Q_DISABLE_COPY_MOVE(FileExplorer)

    void setupList();

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
