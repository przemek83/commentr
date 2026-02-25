#ifndef BROWSEFILESWIDGET_H
#define BROWSEFILESWIDGET_H

#include <QWidget>

class EnhancedLineEdit;
class Explorer;
class File;

namespace Ui
{
class BrowseFilesWidget;
}

class BrowseFilesWidget : public QWidget
{
    Q_OBJECT
public:
    BrowseFilesWidget(bool open, QWidget* parent);

    ~BrowseFilesWidget() override;

private:
    Q_DISABLE_COPY(BrowseFilesWidget)

    Ui::BrowseFilesWidget* ui;

    void initLineEdit();

    void setProperIconForViewButton();

    EnhancedLineEdit* filePathLineEdit_;

    Explorer* currentListView();

private slots:
    void filePathReturnPressed();

    void filePathTextChanged(const QString& arg1);

    void on_changeView_clicked();

    void on_tabWidget_currentChanged(int index);

signals:
    void cancelAction();

    void filePrepared(File* file);
};

#endif  // BROWSEFILESWIDGET_H
