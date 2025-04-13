#ifndef LOCAL_H
#define LOCAL_H

#include <QTableView>

#include "Explorer.h"

class Local : public QTableView, public Explorer
{
    Q_OBJECT
public:
    explicit Local(bool open, QWidget* parent);

    ~Local() override;

    void setPath(QString path) override;

    QString getCurrentPath() override;

    bool fileIsValid(QString file) override;

    bool isWrapping() override;

    void setWrapping(bool wrapping) override;

protected:
    void selectionChanged(const QItemSelection& selected,
                          const QItemSelection& deselected) override;

private:
    Q_DISABLE_COPY(Local)

    void initModelAndView();

    void listViewItemClicked(const QModelIndex& index) override;

    bool timeForSecondClickIsUp_;

private slots:
    void itemDoubleClicked(QModelIndex index);

    void directoryLoaded(const QString& path);

    bool directoryIsAccessible(const QString& path);

    void selectItemOnRelease(QPointF pos);

    void timeForSecondClickIsUp();

signals:
    void fileOrDirNotAccessible(QString name);

    void filePicked(QString file);

    void pathChanged(QString newPath);
};

#endif  // LOCAL_H
