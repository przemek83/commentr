#ifndef EXPLORER_H
#define EXPLORER_H

#include <QString>

class QListView;
class QModelIndex;

class Explorer
{
public:
    explicit Explorer(bool open);

    virtual ~Explorer() = default;

    virtual void setPath(QString path) = 0;

    virtual QString getCurrentPath() = 0;

    /// Check if given file can be used as in/out file.
    virtual bool fileIsValid(QString file) = 0;

    virtual bool isWrappingContent() = 0;

    virtual void initialize() = 0;

    virtual bool initialized() = 0;

    virtual void performOperationOnFile(QString filePath) = 0;

protected:
    virtual QListView* getListView() = 0;

    virtual void listViewItemClicked(const QModelIndex& index) = 0;

    bool open_;

private:
    Q_DISABLE_COPY(Explorer)
};

#endif  // EXPLORER_H
