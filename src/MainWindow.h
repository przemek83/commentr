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
    void on_actionOpen_file_triggered();

    void on_actionSave_file_triggered();

    void on_actionSearch_triggered();

    void on_actionZoom_in_triggered();

    void on_actionZoom_out_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionCopy_triggered();

    void on_actionCut_triggered();

    void on_actionPaste_triggered();

    void on_actionTabsWest_triggered();

    void on_actionTabsEast_triggered();

    void on_actionTabsNorth_triggered();

    void on_actionTabsSouth_triggered();

    void on_actionDecrease50_triggered();

    void on_actionDecrease25_triggered();

    void on_actionDecrease10_triggered();

    void on_actionIncrease10_triggered();

    void on_actionIncrease25_triggered();

    void on_actionIncrease50_triggered();

    void on_actionToolbarFile_Operations_triggered(bool checked);

    void on_actionToolbarCut_copy_paste_triggered(bool checked);

    void on_actionToolbarZoom_in_out_triggered(bool checked);

    void on_actionToolbarUndo_redo_triggered(bool checked);

    void on_actionToolbarSearch_triggered(bool checked);

    void on_actionCheck_spelling_in_comments_triggered(bool checked);

    void on_actionLine_wrap_triggered(bool checked);

    void on_actionExit_triggered();

    void on_actionShowToolbar_triggered(bool checked);

    void on_actionNew_triggered();

    void on_actionSave_as_triggered();

    void on_actionLangC_Cpp_triggered();

    void on_actionLangJava_triggered();

    void on_actionLangObjective_C_triggered();

    void on_actionLangCSharp_triggered();

    void on_actionLangPHP_triggered();

    void on_actionLangVBasic_triggered();

    void on_actionLangPython_triggered();

    void on_actionLangSQL_triggered();

    void on_actionLangJavaScript_triggered();

    void on_actionLangNone_triggered();

    void on_actionAbout_triggered();

    void on_actionToolbarNorth_triggered();

    void on_actionToolbarSouth_triggered();

    void on_actionToolbarWest_triggered();

    void on_actionToolbarEast_triggered();

    void on_actionQt_license_triggered();

    void on_actionShow_hide_keyboard_triggered();

    void on_actionToolbarKeyboard_triggered(bool checked);

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
