#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileInfo>
#include <QMainWindow>
#include <QModelIndex>

#include "Config.h"
#include "EditorTabPage.h"
#include "SpellChecker.h"

namespace Ui
{
class MainWindow;
}  // namespace Ui

class QListView;
class QLabel;
class Highlighter;
class File;
class BrowseFilesWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Config config, SpellChecker spellChecker,
               QWidget* parent = nullptr);
    ~MainWindow() override;

protected:
    void keyReleaseEvent(QKeyEvent* e) override;

    void resizeEvent(QResizeEvent* event) override;

private:
    Q_DISABLE_COPY_MOVE(MainWindow)

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

    void changeModeForCurrentTab(EditorTabPage::EditorMode mode) const;

    void setProperLangActionForMode(EditorTabPage::EditorMode mode);

    void setAvailableFunctionalities(bool visible);

    void changeTabPosition(QTabWidget::TabPosition position);

    void createAndShowBrowseFilesWidget(bool openFileMode);

    void changeSize(float factor);

    void changeToolbarPosition(Qt::ToolBarArea area);

    void setupChangeSizeActions();

    void setupToolbarPositionActions();

    void setupEditorModeActions();

    std::unique_ptr<Ui::MainWindow> ui_;

    int newFileCounter_{0};

    Config config_;

    SpellChecker spellChecker_;

private slots:
    void saveFile();

    void search() const;

    void zoomIn() const;

    void zoomOut() const;

    void undo() const;

    void redo() const;

    void copy();

    void cut();

    void paste() const;

    void toolbarFileOperations(bool checked);

    void toolbarCutCopyPaste(bool checked);

    void toolbarZoomInOut(bool checked);

    void toolbarUndoRedo(bool checked);

    void toolbarSearch(bool checked);

    void checkSpellingInComments(bool checked);

    void lineWrap(bool checked);

    void exitApplication();

    void showToolbar(bool checked);

    void newFile();

    void showAbout();

    void showQtLicense();

    void showHidenKeyboard() const;

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
