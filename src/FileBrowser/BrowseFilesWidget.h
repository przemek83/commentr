#ifndef BROWSEFILESWIDGET_H
#define BROWSEFILESWIDGET_H

#include <memory>

#include <QWidget>

#include "../File.h"
#include "FileAccessMode.h"

class EnhancedLineEdit;
class FileExplorer;
class Config;

namespace Ui
{
class BrowseFilesWidget;
}  // namespace Ui

class BrowseFilesWidget : public QWidget
{
    Q_OBJECT
public:
    BrowseFilesWidget(FileAccessMode mode, Config& config, QWidget* parent);

    ~BrowseFilesWidget() override;

private:
    Q_DISABLE_COPY_MOVE(BrowseFilesWidget)

    void initLineEdit();

    void setProperIconForViewButton();

    FileExplorer* currentListView();

    std::unique_ptr<Ui::BrowseFilesWidget> ui_;

    EnhancedLineEdit* filePathLineEdit_{nullptr};

    Config& config_;

private slots:
    void filePathReturnPressed();

    void filePathTextChanged(const QString& arg1);

    void changeView();

    void currentTabChanged(int index);

signals:
    void cancelAction();

    void filePrepared(File file);
};

#endif  // BROWSEFILESWIDGET_H
