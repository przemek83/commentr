#include "MainWindow.h"

#include <QActionGroup>
#include <QClipboard>
#include <QFileDialog>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStyleFactory>
#include <QTabBar>
#include <QTimer>

#include "CodeViewer.h"
#include "Common.h"
#include "Config.h"
#include "File.h"
#include "FileBrowser/BrowseFilesWidget.h"
#include "Highlighters/Highlighter.h"
#include "ProxyStyle.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow), newFileCounter_(0)
{
    ui_->setupUi(this);

    connectActions();

    showMainPage();

    connect(ui_->tabWidget, &QTabWidget::currentChanged, this,
            &MainWindow::currentTabPageChanged);

    const QClipboard* clipboard = QApplication::clipboard();

    connect(clipboard, &QClipboard::dataChanged, this,
            &MainWindow::clipboardDataChanged);

    initMenus();

    rebuildToolbar();

    Highlighter::setSpellChecking(Config::getInstance().checkSpelling());

    connect(ui_->actionCloseFile, &QAction::triggered, this,
            &MainWindow::closeCurrentTab);

    connect(qApp, &QApplication::focusChanged, this,
            &MainWindow::focusHasChanged);
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::keyReleaseEvent(QKeyEvent* e)
{
    const int key{e->key()};

    if (Qt::Key_Back == key)
    {
        auto page{static_cast<StackedPage>(ui_->stackedWidget->currentIndex())};
        if (PAGE_MAIN != page)
            showMainPage();
        else
            onActionExitTriggered();

        return;
    }

    QMainWindow::keyReleaseEvent(e);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    emit windowResized();
}

void MainWindow::showStatusMsg(QString msg)
{
    QMessageBox::information(this, tr("Message"), msg);
}

void MainWindow::initMenus()
{
    Config& config{Config::getInstance()};

    setupStyles(config);

    setupTabsAlignmentMenu(config);

    setupToolbarActionsMenu(config);

    setupLanguageActionsMenu();

    setupRecentFiles(config);

    ui_->actionCheck_spelling_in_comments->setChecked(config.checkSpelling());
    ui_->actionLine_wrap->setChecked(config.lineWrap());
    ui_->mainToolBar->setVisible(config.showToolbar());
    ui_->actionShowToolbar->setChecked(config.showToolbar());
}

void MainWindow::setupRecentFiles(Config& config)
{
    QMenu* recentMenu{ui_->menuRecent_files};
    recentMenu->clear();

    const QStringList& recentFiles{config.getRecentFiles()};
    if (recentFiles.isEmpty())
    {
        recentMenu->setDisabled(true);
        return;
    }

    recentMenu->setEnabled(true);

    for (const QString& filePath : recentFiles)
    {
        QFileInfo fileInfo(filePath);
        QAction* action{recentMenu->addAction(fileInfo.fileName())};
        action->setData(filePath);
        connect(action, &QAction::triggered, this, &MainWindow::openRecentFile);
    }
}

void MainWindow::setupStyles(const Config& config)
{
    QStringList qtStylesList{QStyleFactory::keys()};
    QMenu* menuStyles{new QMenu(tr("Styles"), ui_->menuOptions)};
    ui_->menuOptions->addAction(menuStyles->menuAction());
    QActionGroup* actionsGroup{new QActionGroup(this)};

    QString styleSetInConfig{config.style()};
    foreach (QString style, qtStylesList)
    {
        QAction* action{new QAction(style, ui_->menuOptions)};
        action->setCheckable(true);
        if (styleSetInConfig == style)
            action->setChecked(true);

        connect(action, &QAction::triggered, this, &MainWindow::qtStylePicked);
        actionsGroup->addAction(action);
    }

    menuStyles->addActions(actionsGroup->actions());
}

void MainWindow::setupTabsAlignmentMenu(const Config& config)
{
    QActionGroup* actionsGroup{new QActionGroup(this)};
    actionsGroup->addAction(ui_->actionTabsNorth);
    actionsGroup->addAction(ui_->actionTabsSouth);
    actionsGroup->addAction(ui_->actionTabsEast);
    actionsGroup->addAction(ui_->actionTabsWest);

    ui_->menuTabs_alignment->addActions(actionsGroup->actions());

    switch (config.getTabPosition())
    {
        case QTabWidget::North:
        {
            ui_->actionTabsNorth->setChecked(true);
            break;
        }

        case QTabWidget::South:
        {
            ui_->actionTabsSouth->setChecked(true);
            break;
        }

        case QTabWidget::West:
        {
            ui_->actionTabsWest->setChecked(true);
            break;
        }

        case QTabWidget::East:
        {
            ui_->actionTabsEast->setChecked(true);
            break;
        }

        default:
        {
            break;
        }
    }

    ui_->tabWidget->setTabPosition(config.getTabPosition());
}

void MainWindow::setupToolbarActionsMenu(const Config& config)
{
    ui_->actionToolbarKeyboard->setChecked(config.toolbarKeyboardAdded());
    ui_->actionToolbarFile_Operations->setChecked(config.toolbarFileAdded());
    ui_->actionToolbarUndo_redo->setChecked(config.toolbarUndoRedoAdded());
    ui_->actionToolbarCut_copy_paste->setChecked(
        config.toolbarCopyPasteCutAdded());
    ui_->actionToolbarZoom_in_out->setChecked(config.toolbarZoomAdded());
    ui_->actionToolbarSearch->setChecked(config.toolbarSearchAdded());

    QActionGroup* actionsGroup{new QActionGroup(this)};
    actionsGroup->addAction(ui_->actionToolbarNorth);
    actionsGroup->addAction(ui_->actionToolbarSouth);
    actionsGroup->addAction(ui_->actionToolbarEast);
    actionsGroup->addAction(ui_->actionToolbarWest);

    ui_->menuToolbarOrientation->addActions(actionsGroup->actions());

    switch (Config::getInstance().toolbarArea())
    {
        case Qt::LeftToolBarArea:
        {
            ui_->actionToolbarWest->setChecked(true);
            break;
        }

        case Qt::RightToolBarArea:
        {
            ui_->actionToolbarEast->setChecked(true);
            break;
        }

        case Qt::TopToolBarArea:
        {
            ui_->actionToolbarNorth->setChecked(true);
            break;
        }

        case Qt::BottomToolBarArea:
        {
            ui_->actionToolbarSouth->setChecked(true);
            break;
        }

        default:
        {
            break;
        }
    }

    Qt::ToolBarArea area = config.toolbarArea();
    if (toolBarArea(ui_->mainToolBar) != area)
    {
        addToolBar(area, ui_->mainToolBar);
    }
}

void MainWindow::setupLanguageActionsMenu()
{
    QActionGroup* actionsGroup{new QActionGroup(this)};
    actionsGroup->addAction(ui_->actionLangCSharp);
    actionsGroup->addAction(ui_->actionLangC_Cpp);
    actionsGroup->addAction(ui_->actionLangJava);
    actionsGroup->addAction(ui_->actionLangJavaScript);
    actionsGroup->addAction(ui_->actionLangNone);
    actionsGroup->addAction(ui_->actionLangObjective_C);
    actionsGroup->addAction(ui_->actionLangPHP);
    actionsGroup->addAction(ui_->actionLangPython);
    actionsGroup->addAction(ui_->actionLangSQL);
    actionsGroup->addAction(ui_->actionLangVBasic);

    ui_->menuLanguage_mode->addActions(actionsGroup->actions());
}

void MainWindow::showMainPage()
{
    setAvailableFunctionalitiesForMainWindow(true);
    QWidget* fileBrowser{ui_->stackedWidget->widget(PAGE_FILE_BROWSER)};
    ui_->stackedWidget->setCurrentIndex(PAGE_MAIN);
    if (fileBrowser != nullptr)
    {
        ui_->stackedWidget->removeWidget(fileBrowser);

        // Delete later used because of problems with deleting object from slot
        // in different object called by first one.
        fileBrowser->deleteLater();
    }
}

void MainWindow::createNewTab(File* file)
{
    Config& config{Config::getInstance()};

    showMainPage();

    EditorTabPage* editorTabPage{
        new EditorTabPage(file, config.fontSize(), ui_->tabWidget)};

    editorTabPage->setLineWrap(config.lineWrap());

    connect(editorTabPage, &EditorTabPage::redoIsAvailable, this,
            &MainWindow::redoAvailabilityChanged);

    connect(editorTabPage, &EditorTabPage::undoIsAvailable, this,
            &MainWindow::undoAvailabilityChanged);

    connect(editorTabPage, &EditorTabPage::copyCutIsAvailable, this,
            &MainWindow::copyAndCutAvailabilityChanged);

    int newestIndex = ui_->tabWidget->addTab(editorTabPage, file->baseName());

    ui_->tabWidget->setCurrentIndex(newestIndex);

    manageActions(true);

    if (file->source() == Common::SOURCE_LOCAL)
    {
        config.addFilePathToRecentFiles(file->getFilePath());

        setupRecentFiles(config);
    }
}

void MainWindow::saveFileFromTab(File* file)
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab != nullptr)
    {
        QString newBaseName = file->baseName();
        switch (file->source())
        {
            case Common::SOURCE_LOCAL:
            {
                QString content(currentTab->getCurrentText());
                showStatusMsg(Common::saveFile(file->getFilePath(), content));
                currentTab->changeFile(file);
                delete file;
                break;
            }

            default:
            {
                delete file;
                break;
            }
        }

        ui_->tabWidget->setTabText(ui_->tabWidget->currentIndex(), newBaseName);
    }

    showMainPage();
}

void MainWindow::openRecentFile()
{
    QAction* action{::qobject_cast<QAction*>(sender())};
    if (action != nullptr)
    {
        QString filePath = action->data().toString();

        QFileInfo fileInfo(filePath);

        if (!fileInfo.exists())
        {
            showStatusMsg("File " + filePath + " not found.");
            return;
        }

        if (!fileInfo.isReadable())
        {
            showStatusMsg("File " + filePath + " not readable.");
            return;
        }

        File* file{new File(Common::SOURCE_LOCAL,
                            File::filePathToPath(filePath),
                            File::filePathToBaseName(filePath),
                            File::filePathToSuffix(filePath),
                            new QString(Common::loadFile(filePath)))};

        createNewTab(file);
    }
}

void MainWindow::connectActions()
{
    connect(ui_->actionOpen_file, &QAction::triggered,
            [this]() { createAndShowBrowseFilesWidget(true); });
    connect(ui_->actionSave_file, &QAction::triggered, this,
            &MainWindow::onActionSaveFileTriggered);
    connect(ui_->actionSearch, &QAction::triggered, this,
            &MainWindow::onActionSearchTriggered);
    connect(ui_->actionZoom_in, &QAction::triggered, this,
            &MainWindow::onActionZoomInTriggered);
    connect(ui_->actionZoom_out, &QAction::triggered, this,
            &MainWindow::onActionZoomOutTriggered);
    connect(ui_->actionUndo, &QAction::triggered, this,
            &MainWindow::onActionUndoTriggered);
    connect(ui_->actionRedo, &QAction::triggered, this,
            &MainWindow::onActionRedoTriggered);
    connect(ui_->actionCopy, &QAction::triggered, this,
            &MainWindow::onActionCopyTriggered);
    connect(ui_->actionCut, &QAction::triggered, this,
            &MainWindow::onActionCutTriggered);
    connect(ui_->actionPaste, &QAction::triggered, this,
            &MainWindow::onActionPasteTriggered);
    connect(ui_->actionTabsWest, &QAction::triggered,
            [this]() { changeTabPosition(QTabWidget::West); });
    connect(ui_->actionTabsEast, &QAction::triggered,
            [this]() { changeTabPosition(QTabWidget::East); });
    connect(ui_->actionTabsNorth, &QAction::triggered,
            [this]() { changeTabPosition(QTabWidget::North); });
    connect(ui_->actionTabsSouth, &QAction::triggered,
            [this]() { changeTabPosition(QTabWidget::South); });
    connect(ui_->actionDecrease50, &QAction::triggered, this,
            &MainWindow::onActionDecrease50Triggered);
    connect(ui_->actionDecrease25, &QAction::triggered, this,
            &MainWindow::onActionDecrease25Triggered);
    connect(ui_->actionDecrease10, &QAction::triggered, this,
            &MainWindow::onActionDecrease10Triggered);
    connect(ui_->actionIncrease10, &QAction::triggered, this,
            &MainWindow::onActionIncrease10Triggered);
    connect(ui_->actionIncrease25, &QAction::triggered, this,
            &MainWindow::onActionIncrease25Triggered);
    connect(ui_->actionIncrease50, &QAction::triggered, this,
            &MainWindow::onActionIncrease50Triggered);
    connect(ui_->actionToolbarFile_Operations, &QAction::triggered, this,
            &MainWindow::onActionToolbarFileOperationsTriggered);
    connect(ui_->actionToolbarCut_copy_paste, &QAction::triggered, this,
            &MainWindow::onActionToolbarCutCopyPasteTriggered);
    connect(ui_->actionToolbarZoom_in_out, &QAction::triggered, this,
            &MainWindow::onActionToolbarZoomInOutTriggered);
    connect(ui_->actionToolbarUndo_redo, &QAction::triggered, this,
            &MainWindow::onActionToolbarUndoRedoTriggered);
    connect(ui_->actionToolbarSearch, &QAction::triggered, this,
            &MainWindow::onActionToolbarSearchTriggered);
    connect(ui_->actionCheck_spelling_in_comments, &QAction::triggered, this,
            &MainWindow::onActionCheckSpellingInCommentsTriggered);
    connect(ui_->actionLine_wrap, &QAction::triggered, this,
            &MainWindow::onActionLineWrapTriggered);
    connect(ui_->actionExit, &QAction::triggered, this,
            &MainWindow::onActionExitTriggered);
    connect(ui_->actionShowToolbar, &QAction::triggered, this,
            &MainWindow::onActionShowToolbarTriggered);
    connect(ui_->actionNew, &QAction::triggered, this,
            &MainWindow::onActionNewTriggered);
    connect(ui_->actionSave_as, &QAction::triggered,
            [this]() { createAndShowBrowseFilesWidget(false); });
    connect(ui_->actionLangC_Cpp, &QAction::triggered, [this]()
            { changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_C_CPP); });
    connect(ui_->actionLangJava, &QAction::triggered, [this]()
            { changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_JAVA); });
    connect(
        ui_->actionLangObjective_C, &QAction::triggered, [this]()
        { changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_OBJECTIVE_C); });
    connect(ui_->actionLangCSharp, &QAction::triggered, [this]()
            { changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_C_SHARP); });
    connect(ui_->actionLangPHP, &QAction::triggered, [this]()
            { changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_PHP); });
    connect(
        ui_->actionLangVBasic, &QAction::triggered, [this]()
        { changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_VISUAL_BASIC); });
    connect(ui_->actionLangPython, &QAction::triggered, [this]()
            { changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_PYTHON); });
    connect(ui_->actionLangSQL, &QAction::triggered, [this]()
            { changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_SQL); });
    connect(
        ui_->actionLangJavaScript, &QAction::triggered, [this]()
        { changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_JAVASCRIPT); });
    connect(
        ui_->actionLangNone, &QAction::triggered, [this]()
        { changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_PLAIN_TEXT); });
    connect(ui_->actionAbout, &QAction::triggered, this,
            &MainWindow::onActionAboutTriggered);
    connect(ui_->actionToolbarNorth, &QAction::triggered, this,
            &MainWindow::onActionToolbarNorthTriggered);
    connect(ui_->actionToolbarSouth, &QAction::triggered, this,
            &MainWindow::onActionToolbarSouthTriggered);
    connect(ui_->actionToolbarWest, &QAction::triggered, this,
            &MainWindow::onActionToolbarWestTriggered);
    connect(ui_->actionToolbarEast, &QAction::triggered, this,
            &MainWindow::onActionToolbarEastTriggered);
    connect(ui_->actionQt_license, &QAction::triggered, this,
            &MainWindow::onActionQtLicenseTriggered);
    connect(ui_->actionShow_hide_keyboard, &QAction::triggered, this,
            &MainWindow::onActionShowHideKeyboardTriggered);
    connect(ui_->actionToolbarKeyboard, &QAction::triggered, this,
            &MainWindow::onActionToolbarKeyboardTriggered);
}

void MainWindow::manageActions(bool tabExist)
{
    ui_->actionSearch->setEnabled(tabExist);
    ui_->actionZoom_in->setEnabled(tabExist);
    ui_->actionZoom_out->setEnabled(tabExist);
    ui_->actionCloseFile->setEnabled(tabExist);
    ui_->actionSave_file->setEnabled(tabExist);
    ui_->actionSave_as->setEnabled(tabExist);
}

void MainWindow::closeCurrentTab()
{
    int currentIndex{ui_->tabWidget->currentIndex()};

    QWidget* tabToDelete{ui_->tabWidget->widget(currentIndex)};

    if (tabToDelete == nullptr)
        return;

    QString msg{tr("Close ") + ui_->tabWidget->tabText(currentIndex) + "?"};
    QMessageBox::StandardButton answer =
        QMessageBox::question(this, tr("Confirm"), msg);

    if (answer == QMessageBox::Yes)
    {
        ui_->tabWidget->removeTab(currentIndex);
        delete tabToDelete;

        if (ui_->tabWidget->count() == 0)
            manageActions(false);
    }
}

void MainWindow::qtStylePicked()
{
    QAction* action{dynamic_cast<QAction*>(sender())};
    if (action != nullptr)
    {
        QString style{action->text()};
        QWidget* focusWidget{qApp->focusWidget()};
        if (focusWidget != nullptr)
            focusWidget->clearFocus();

        qApp->setStyleSheet(QString());
        qApp->setStyle(new QProxyStyle(style));
    }
}

void MainWindow::rebuildToolbar()
{
    ui_->mainToolBar->clear();

    ui_->mainToolBar->addAction(ui_->actionMenu);

    const Config& config{Config::getInstance()};

    if (config.toolbarKeyboardAdded())
        ui_->mainToolBar->addAction(ui_->actionShow_hide_keyboard);

    if (config.toolbarFileAdded())
    {
        ui_->mainToolBar->addAction(ui_->actionNew);
        ui_->mainToolBar->addAction(ui_->actionOpen_file);
        ui_->mainToolBar->addAction(ui_->actionSave_file);
        ui_->mainToolBar->addAction(ui_->actionSave_as);
        ui_->mainToolBar->addAction(ui_->actionCloseFile);
    }

    if (config.toolbarUndoRedoAdded())
    {
        ui_->mainToolBar->addAction(ui_->actionUndo);
        ui_->mainToolBar->addAction(ui_->actionRedo);
    }

    if (config.toolbarCopyPasteCutAdded())
    {
        ui_->mainToolBar->addAction(ui_->actionCopy);
        ui_->mainToolBar->addAction(ui_->actionCut);
        ui_->mainToolBar->addAction(ui_->actionPaste);
    }

    if (config.toolbarZoomAdded())
    {
        ui_->mainToolBar->addAction(ui_->actionZoom_in);
        ui_->mainToolBar->addAction(ui_->actionZoom_out);
    }

    if (config.toolbarSearchAdded())
    {
        ui_->mainToolBar->addAction(ui_->actionSearch);
    }
}

void MainWindow::onActionSearchTriggered()
{
    EditorTabPage* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->flipFindVisibility();
}

void MainWindow::onActionZoomInTriggered()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->zoom(true);
}

void MainWindow::onActionZoomOutTriggered()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->zoom(false);
}

void MainWindow::currentTabPageChanged(int index)
{
    QTabBar* tabBar{ui_->tabWidget->findChild<QTabBar*>()};
    tabBar->setVisible(1 < ui_->tabWidget->count());

    if (index != -1)
    {
        auto* currentTab{
            dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

        if (currentTab != nullptr)
        {
            ui_->actionRedo->setEnabled(currentTab->redoAvailable());
            ui_->actionUndo->setEnabled(currentTab->undoAvailable());
            ui_->actionCopy->setEnabled(!currentTab->selectionEmpty());
            ui_->actionCut->setEnabled(!currentTab->selectionEmpty());
            clipboardDataChanged();
            setProperLangActionForMode(currentTab->mode());
            ui_->menuLanguage_mode->setEnabled(true);
            return;
        }
    }

    ui_->actionRedo->setEnabled(false);
    ui_->actionUndo->setEnabled(false);
    ui_->actionCopy->setEnabled(false);
    ui_->actionCut->setEnabled(false);
    ui_->actionPaste->setEnabled(false);
    ui_->menuLanguage_mode->setEnabled(false);
}

void MainWindow::redoAvailabilityChanged(bool available)
{
    ui_->actionRedo->setEnabled(available);
}

void MainWindow::undoAvailabilityChanged(bool available)
{
    ui_->actionUndo->setEnabled(available);
}

void MainWindow::onActionUndoTriggered()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->undo();
}

void MainWindow::onActionRedoTriggered()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->redo();
}

void MainWindow::clipboardDataChanged()
{
    const QClipboard* clipboard{QApplication::clipboard()};

    ui_->actionPaste->setEnabled(!clipboard->text().isEmpty());
}

void MainWindow::copyAndCutAvailabilityChanged(bool available)
{
    ui_->actionCopy->setEnabled(available);
    ui_->actionCut->setEnabled(available);
}

void MainWindow::onActionCopyTriggered()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab != nullptr)
    {
        currentTab->copy();
        ui_->actionPaste->setEnabled(true);
    }
}

void MainWindow::onActionCutTriggered()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab != nullptr)
    {
        currentTab->cut();
        ui_->actionPaste->setEnabled(true);
    }
}

void MainWindow::onActionPasteTriggered()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->paste();
}

void MainWindow::changeTabPosition(QTabWidget::TabPosition position)
{
    ui_->tabWidget->setTabPosition(position);
    Config::getInstance().setTabPosition(position);

    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->refreshVisualIndicators();
}

void MainWindow::createAndShowBrowseFilesWidget(bool openFileMode)
{
    setAvailableFunctionalitiesForMainWindow(false);

    auto* browseFilesWidget{
        new BrowseFilesWidget(openFileMode, ui_->stackedWidget)};

    connect(browseFilesWidget, &BrowseFilesWidget::cancelAction, this,
            &MainWindow::showMainPage);

    if (openFileMode)
        connect(browseFilesWidget, &BrowseFilesWidget::filePrepared, this,
                &MainWindow::createNewTab);
    else
        connect(browseFilesWidget, &BrowseFilesWidget::filePrepared, this,
                &MainWindow::saveFileFromTab);

    ui_->stackedWidget->insertWidget(PAGE_FILE_BROWSER, browseFilesWidget);

    ui_->stackedWidget->setCurrentIndex(PAGE_FILE_BROWSER);
}

void MainWindow::onActionDecrease50Triggered()
{
    float currentSize = Config::getInstance().uiSize();
    Config::getInstance().setUiSize(currentSize * .5);
    ProxyStyle::updateUisize();
}

void MainWindow::onActionDecrease25Triggered()
{
    float currentSize = Config::getInstance().uiSize();
    Config::getInstance().setUiSize(currentSize * .75);
    ProxyStyle::updateUisize();
}

void MainWindow::onActionDecrease10Triggered()
{
    float currentSize = Config::getInstance().uiSize();
    Config::getInstance().setUiSize(currentSize * .90);
    ProxyStyle::updateUisize();
}

void MainWindow::onActionIncrease10Triggered()
{
    float currentSize = Config::getInstance().uiSize();
    Config::getInstance().setUiSize(currentSize * 1.1);
    ProxyStyle::updateUisize();
}

void MainWindow::onActionIncrease25Triggered()
{
    float currentSize = Config::getInstance().uiSize();
    Config::getInstance().setUiSize(currentSize * 1.25);
    ProxyStyle::updateUisize();
}

void MainWindow::onActionIncrease50Triggered()
{
    float currentSize = Config::getInstance().uiSize();
    Config::getInstance().setUiSize(currentSize * 1.5);
    ProxyStyle::updateUisize();
}

void MainWindow::onActionToolbarKeyboardTriggered(bool checked)
{
    Config::getInstance().setToolbarKeyboardAdded(checked);
    rebuildToolbar();
}

void MainWindow::onActionToolbarFileOperationsTriggered(bool checked)
{
    Config::getInstance().setToolbarFileAdded(checked);
    rebuildToolbar();
}

void MainWindow::onActionToolbarCutCopyPasteTriggered(bool checked)
{
    Config::getInstance().setToolbarCopyPasteCutAdded(checked);
    rebuildToolbar();
}

void MainWindow::onActionToolbarZoomInOutTriggered(bool checked)
{
    Config::getInstance().setToolbarZoomAdded(checked);
    rebuildToolbar();
}

void MainWindow::onActionToolbarUndoRedoTriggered(bool checked)
{
    Config::getInstance().setToolbarUndoRedoAdded(checked);
    rebuildToolbar();
}

void MainWindow::onActionToolbarSearchTriggered(bool checked)
{
    Config::getInstance().setToolbarSearchAdded(checked);
    rebuildToolbar();
}

void MainWindow::onActionCheckSpellingInCommentsTriggered(bool checked)
{
    Config::getInstance().setCheckSpelling(checked);

    Highlighter::setSpellChecking(checked);

    QList<Highlighter*> highlighters{findChildren<Highlighter*>()};
    foreach (Highlighter* highlighter, highlighters)
        highlighter->rehighlight();
}

void MainWindow::onActionLineWrapTriggered(bool checked)
{
    Config::getInstance().setLineWrap(checked);

    QList<EditorTabPage*> pages{ui_->tabWidget->findChildren<EditorTabPage*>()};
    foreach (EditorTabPage* page, pages)
        page->setLineWrap(checked);
}

void MainWindow::onActionExitTriggered()
{
    QMessageBox::StandardButton answer{
        QMessageBox::question(this, tr("Quit"), tr("Quit CommentR?"))};

    if (answer == QMessageBox::No)
        return;

    QCoreApplication::quit();
}

void MainWindow::onActionSaveFileTriggered()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab != nullptr)
    {
        File* file{currentTab->getCurrentFileCopy()};
        switch (file->source())
        {
            case Common::SOURCE_NOT_SET:
            {
                createAndShowBrowseFilesWidget(false);
                break;
            }

            case Common::SOURCE_LOCAL:
            {
                showStatusMsg(
                    Common::saveFile(file->getFilePath(), *(file->content())));
                delete file;

                break;
            }

            default:
            {
                break;
            }
        }
    }
}

void MainWindow::onActionShowToolbarTriggered(bool checked)
{
    Config::getInstance().setShowToolbar(checked);
    ui_->mainToolBar->setVisible(checked);
}

void MainWindow::changeModeForCurrentTab(EditorTabPage::EditorMode mode)
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->setMode(mode);
}

void MainWindow::setProperLangActionForMode(EditorTabPage::EditorMode mode)
{
    switch (mode)
    {
        case EditorTabPage::EDITOR_MODE_C_CPP:
        {
            ui_->actionLangC_Cpp->setChecked(true);
            break;
        }

        case EditorTabPage::EDITOR_MODE_JAVA:
        {
            ui_->actionLangJava->setChecked(true);
            break;
        }

        case EditorTabPage::EDITOR_MODE_OBJECTIVE_C:
        {
            ui_->actionLangObjective_C->setChecked(true);
            break;
        }

        case EditorTabPage::EDITOR_MODE_C_SHARP:
        {
            ui_->actionLangCSharp->setChecked(true);
            break;
        }

        case EditorTabPage::EDITOR_MODE_PHP:
        {
            ui_->actionLangPHP->setChecked(true);
            break;
        }

        case EditorTabPage::EDITOR_MODE_VISUAL_BASIC:
        {
            ui_->actionLangVBasic->setChecked(true);
            break;
        }

        case EditorTabPage::EDITOR_MODE_PYTHON:
        {
            ui_->actionLangPython->setChecked(true);
            break;
        }

        case EditorTabPage::EDITOR_MODE_SQL:
        {
            ui_->actionLangSQL->setChecked(true);
            break;
        }

        case EditorTabPage::EDITOR_MODE_JAVASCRIPT:
        {
            ui_->actionLangJavaScript->setChecked(true);
            break;
        }

        case EditorTabPage::EDITOR_MODE_PLAIN_TEXT:
        {
            ui_->actionLangNone->setChecked(true);
            break;
        }

        default:
        {
            break;
        }
    }
}

void MainWindow::onActionToolbarNorthTriggered()
{
    Config::getInstance().setToolbarArea(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, ui_->mainToolBar);
}

void MainWindow::onActionToolbarSouthTriggered()
{
    Config::getInstance().setToolbarArea(Qt::BottomToolBarArea);
    addToolBar(Qt::BottomToolBarArea, ui_->mainToolBar);
}

void MainWindow::onActionToolbarWestTriggered()
{
    Config::getInstance().setToolbarArea(Qt::LeftToolBarArea);
    addToolBar(Qt::LeftToolBarArea, ui_->mainToolBar);
}

void MainWindow::onActionToolbarEastTriggered()
{
    Config::getInstance().setToolbarArea(Qt::RightToolBarArea);
    addToolBar(Qt::RightToolBarArea, ui_->mainToolBar);
}

void MainWindow::onActionNewTriggered()
{
    showMainPage();
    ++newFileCounter_;
    File* file{new File(Common::SOURCE_NOT_SET, "",
                        tr("File") + QString::number(newFileCounter_), "",
                        new QString(""))};

    createNewTab(file);
}

void MainWindow::onActionAboutTriggered()
{
    File* file{new File(Common::SOURCE_NOT_SET, "",
                        "About " + QCoreApplication::applicationName(), "",
                        new QString(Common::loadFile(":/about.txt")))};

    createNewTab(file);
}

void MainWindow::onActionQtLicenseTriggered()
{
    File* file{new File(Common::SOURCE_NOT_SET, "", "Qt license", "",
                        new QString(Common::loadFile(":/LICENSE")))};

    createNewTab(file);
}

void MainWindow::onActionShowHideKeyboardTriggered()
{
    if (ui_->stackedWidget->currentIndex() != PAGE_MAIN)
        return;

    QInputMethod* input{QGuiApplication::inputMethod()};
    input->setVisible(!input->isVisible());
}

void MainWindow::setAvailableFunctionalitiesForMainWindow(bool visible)
{
    if (visible)
        ui_->mainToolBar->setVisible(ui_->actionShowToolbar->isChecked());
    else
        ui_->mainToolBar->setVisible(false);

    ui_->menuEdit->setEnabled(visible);
    ui_->actionShowToolbar->setEnabled(visible);
    ui_->menuHelp->setEnabled(visible);
}

void MainWindow::focusHasChanged([[maybe_unused]] QWidget* old, QWidget* now)
{
    auto* codeViewer{dynamic_cast<CodeViewer*>(now)};
    auto* lineEdit{dynamic_cast<QLineEdit*>(now)};
    bool keyboardFocusWidget{lineEdit != nullptr || codeViewer != nullptr};
    ui_->actionShow_hide_keyboard->setEnabled(keyboardFocusWidget);
}

void MainWindow::on_actionMenu_triggered()
{
    QMenu menu;
    menu.addMenu(ui_->menuFile);
    menu.addMenu(ui_->menuEdit);
    menu.addMenu(ui_->menuOptions);
    menu.addMenu(ui_->menuHelp);
    menu.exec(QCursor::pos());
}
