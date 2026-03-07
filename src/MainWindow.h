#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileInfo>
#include <QMainWindow>
#include <QModelIndex>

#include "EditorTabPage.h"

namespace Ui
{
class MainWindow;
}

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

    Ui::MainWindow* ui;

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

    // Change language for current tab
    void changeModeForCurrentTab(EditorTabPage::EditorMode mode);

    void setProperLangActionForMode(EditorTabPage::EditorMode mode);

    void setAvailableFunctionalitiesForMainWindow(bool visible);

    void changeTabPosition(QTabWidget::TabPosition position);

    void createAndShowBrowseFilesWidget(bool openFileMode);

    /// Counter used at new file creation.
    int newFileCounter_;

private slots:
    void onActionOpenFileTriggered();

    void onActionSaveFileTriggered();

    void onActionSearchTriggered();

    void onActionZoomInTriggered();

    void onActionZoomOutTriggered();

    void onActionUndoTriggered();

    void onActionRedoTriggered();

    void onActionCopyTriggered();

    void onActionCutTriggered();

    void onActionPasteTriggered();

    void onActionTabsWestTriggered();

    void onActionTabsEastTriggered();

    void onActionTabsNorthTriggered();

    void onActionTabsSouthTriggered();

    void onActionDecrease50Triggered();

    void onActionDecrease25Triggered();

    void onActionDecrease10Triggered();

    void onActionIncrease10Triggered();

    void onActionIncrease25Triggered();

    void onActionIncrease50Triggered();

    void onActionToolbarFileOperationsTriggered(bool checked);

    void onActionToolbarCutCopyPasteTriggered(bool checked);

    void onActionToolbarZoomInOutTriggered(bool checked);

    void onActionToolbarUndoRedoTriggered(bool checked);

    void onActionToolbarSearchTriggered(bool checked);

    void onActionCheckSpellingInCommentsTriggered(bool checked);

    void onActionLineWrapTriggered(bool checked);

    void onActionExitTriggered();

    void onActionShowToolbarTriggered(bool checked);

    void onActionNewTriggered();

    void onActionSaveAsTriggered();

    void onActionLangCppCTriggered();

    void onActionLangJavaTriggered();

    void onActionLangObjectiveCTriggered();

    void onActionLangCSharpTriggered();

    void onActionLangPHPTriggered();

    void onActionLangVBasicTriggered();

    void onActionLangPythonTriggered();

    void onActionLangSqlTriggered();

    void onActionLangJavaScriptTriggered();

    void onActionLangNoneTriggered();

    void onActionAboutTriggered();

    void onActionToolbarNorthTriggered();

    void onActionToolbarSouthTriggered();

    void onActionToolbarWestTriggered();

    void onActionToolbarEastTriggered();

    void onActionQtLicenseTriggered();

    void onActionShowHideKeyboardTriggered();

    void onActionToolbarKeyboardTriggered(bool checked);

    void closeCurrentTab();

    void qtStylePicked();

    void currentTabPageChanged(int index);

    void redoAvailabilityChanged(bool available);

    void undoAvailabilityChanged(bool available);

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
