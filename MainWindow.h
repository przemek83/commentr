#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QFileInfo>

#include "EditorTabPage.h"

namespace Ui {
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
    explicit MainWindow(QWidget *parent = 0);

    virtual ~MainWindow();

protected:
    virtual void keyReleaseEvent(QKeyEvent* e);

    virtual void resizeEvent(QResizeEvent* event);

private:
    Q_DISABLE_COPY(MainWindow)

    Ui::MainWindow *ui;

    /**
     * @brief create tab for FTP connection setup 9host, login, password).
     */
    void initFtpConnectionSetup();

    void manageActions(bool tabExist);

    enum StackedPage
    {
        PAGE_MAIN = 0,
        PAGE_CONNECTION_SETUP,
        PAGE_FILE_BROWSER
    };

    void initMenus();

    void setupTabsAlignmentMenu(const Config& config);

    void setupToolbarActionsMenu(const Config& config);

    void setupLanguageActionsMenu();

    void setupRecentFiles(Config& config);

    void setupStyles(const Config& config);

    void rebuildToolbar();

    /**
     * @brief change language for current tab
     * @param mode new programming language.
     */
    void changeModeForCurrentTab(EditorTabPage::EditorMode mode);

    /**
     * @brief set proper action checked according to given language/mode.
     * @param mode language set.
     */
    void setProperLangActionForMode(EditorTabPage::EditorMode mode);

    /**
     * @brief if main window is shown activate proper functionalities.
     * @param visible main window is visible.
     */
    void setAvailableFunctionalitiesForMainWindow(bool visible);

    /**
     * @brief set new tabbar position.
     * @param position new position.
     */
    void changeTabPosition(QTabWidget::TabPosition position);

    /**
     * @brief create and show BrowseFilesWidget for given mode (open or save).
     * @param openFileMode true = open, false = save.
     */
    void createAndShowBrowseFilesWidget(bool openFileMode);

    //QInputMethod isVisible() method is not working. This is workaround.
    bool virtualKeyboardVisible_;

    ///Counter used at new file creation.
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

    void on_actionQT_LGPL_license_triggered();

    void on_actionShow_hide_keyboard_triggered();

    void on_actionToolbarKeyboard_triggered(bool checked);

    void on_actionKeyboard_after_tap_triggered(bool checked);

    void on_actionConnection_setup_triggered();

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

    /**
     * @brief create new tab using give File.
     * @param file file to use in new tab.
     */
    void createNewTab(File* file);

    /**
     * @brief save given file aacording to destination.
     * @param file given file.
     */
    void saveFileFromTab(File* file);

    void saveFtpFile(File* file);

    /**
     * @brief inform MainWindow when FTP saves file.
     * @param success true if saved, false when error (displayed already).
     */
    void fileSavedUsingFtp(bool success);

    void openRecentFile();

signals:
    void windowResized();
};

#endif // MAINWINDOW_H
