#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileInfo>
#include <QMainWindow>
#include <QModelIndex>

#include "EditorTabPage.h"

namespace Ui
{
class MainWindow;
}  // namespace Ui

class QListView;
class QLabel;
class Highlighter;
class File;
class BrowseFilesWidget;
class Config;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow() override;

protected:
    void keyReleaseEvent(QKeyEvent* e) override;

    void resizeEvent(QResizeEvent* event) override;

private:
    Q_DISABLE_COPY(MainWindow)

    Ui::MainWindow* ui_;

    void connectActions();

    void manageActions(bool tabExist);

    enum StackedPage
    {
        PAGE_MAIN = 0,
        PAGE_FILE_BROWSER,
    };

    void initMenus();

    void setupTabsAlignmentMenu(const Config& config);

    void setupToolbarActionsMenu(const Config& config);

    void setupLanguageActionsMenu();

    void setupRecentFiles(Config& config);

    void setupStyles(const Config& config);

    void rebuildToolbar();

    void changeModeForCurrentTab(EditorTabPage::EditorMode mode);

    void setProperLangActionForMode(EditorTabPage::EditorMode mode);

    void setAvailableFunctionalities(bool visible);

    void changeTabPosition(QTabWidget::TabPosition position);

    void createAndShowBrowseFilesWidget(bool openFileMode);

    static void changeSize(float factor);

    void changeToolbarPosition(Qt::ToolBarArea area);

    void setupChangeSizeActions();

    void setupToolbarPositionActions();

    void setupEditorModeActions();

    int newFileCounter_;

private slots:
    void saveFile();

    void search();

    void zoomIn();

    void zoomOut();

    void undo();

    void redo();

    void copy();

    void cut();

    void paste();

    void toolbarFileOperations(bool checked);

    void toolbarCutCopyPaste(bool checked);

    void toolbarZoomInOut(bool checked);

    void toolbarUndoRedo(bool checked);

    void toolbarSearch(bool checked);

    void checkSpellingInComments(bool checked);

    void lineWrap(bool checked);

    void exit();

    void showToolbar(bool checked);

    void newFile();

    void showAbout();

    void showQtLicense();

    void showHidenKeyboard();

    void toolbarKeyboardActivated(bool checked);

    void closeCurrentTab();

    void qtStylePicked();

    void currentTabPageChanged(int index);

    void clipboardDataChanged();

    void copyAndCutAvailabilityChanged(bool available);

    void showMainPage();

    void showStatusMsg(QString msg);

    void focusHasChanged(QWidget* old, QWidget* now);

    void createNewTab(File* file);

    void saveFileFromTab(File* file);

    void openRecentFile();

    void on_actionMenu_triggered();

signals:
    void windowResized();
};

#endif  // MAINWINDOW_H
