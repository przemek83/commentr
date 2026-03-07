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
    : QMainWindow(parent), ui(new Ui::MainWindow), newFileCounter_(0)
{
    ui->setupUi(this);

    connectActions();

    showMainPage();

    connect(ui->tabWidget, &QTabWidget::currentChanged, this,
            &MainWindow::currentTabPageChanged);

    const QClipboard* clipboard = QApplication::clipboard();

    connect(clipboard, &QClipboard::dataChanged, this,
            &MainWindow::clipboardDataChanged);

    initMenus();

    rebuildToolbar();

    Highlighter::setSpellChecking(Config::getInstance().checkSpelling());

    connect(ui->actionCloseFile, &QAction::triggered, this,
            &MainWindow::closeCurrentTab);

    connect(qApp, &QApplication::focusChanged, this,
            &MainWindow::focusHasChanged);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::keyReleaseEvent(QKeyEvent* e)
{
    const int key{e->key()};

    if (Qt::Key_Back == key)
    {
        auto page{static_cast<StackedPage>(ui->stackedWidget->currentIndex())};
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

    ui->actionCheck_spelling_in_comments->setChecked(config.checkSpelling());
    ui->actionLine_wrap->setChecked(config.lineWrap());
    ui->mainToolBar->setVisible(config.showToolbar());
    ui->actionShowToolbar->setChecked(config.showToolbar());
}

void MainWindow::setupRecentFiles(Config& config)
{
    QMenu* recentMenu{ui->menuRecent_files};
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
    QMenu* menuStyles{new QMenu(tr("Styles"), ui->menuOptions)};
    ui->menuOptions->addAction(menuStyles->menuAction());
    QActionGroup* actionsGroup{new QActionGroup(this)};

    QString styleSetInConfig{config.style()};
    foreach (QString style, qtStylesList)
    {
        QAction* action{new QAction(style, ui->menuOptions)};
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
    actionsGroup->addAction(ui->actionTabsNorth);
    actionsGroup->addAction(ui->actionTabsSouth);
    actionsGroup->addAction(ui->actionTabsEast);
    actionsGroup->addAction(ui->actionTabsWest);

    ui->menuTabs_alignment->addActions(actionsGroup->actions());

    switch (config.getTabPosition())
    {
        case QTabWidget::North:
        {
            ui->actionTabsNorth->setChecked(true);
            break;
        }

        case QTabWidget::South:
        {
            ui->actionTabsSouth->setChecked(true);
            break;
        }

        case QTabWidget::West:
        {
            ui->actionTabsWest->setChecked(true);
            break;
        }

        case QTabWidget::East:
        {
            ui->actionTabsEast->setChecked(true);
            break;
        }

        default:
        {
            break;
        }
    }

    ui->tabWidget->setTabPosition(config.getTabPosition());
}

void MainWindow::setupToolbarActionsMenu(const Config& config)
{
    ui->actionToolbarKeyboard->setChecked(config.toolbarKeyboardAdded());
    ui->actionToolbarFile_Operations->setChecked(config.toolbarFileAdded());
    ui->actionToolbarUndo_redo->setChecked(config.toolbarUndoRedoAdded());
    ui->actionToolbarCut_copy_paste->setChecked(
        config.toolbarCopyPasteCutAdded());
    ui->actionToolbarZoom_in_out->setChecked(config.toolbarZoomAdded());
    ui->actionToolbarSearch->setChecked(config.toolbarSearchAdded());

    QActionGroup* actionsGroup{new QActionGroup(this)};
    actionsGroup->addAction(ui->actionToolbarNorth);
    actionsGroup->addAction(ui->actionToolbarSouth);
    actionsGroup->addAction(ui->actionToolbarEast);
    actionsGroup->addAction(ui->actionToolbarWest);

    ui->menuToolbarOrientation->addActions(actionsGroup->actions());

    switch (Config::getInstance().toolbarArea())
    {
        case Qt::LeftToolBarArea:
        {
            ui->actionToolbarWest->setChecked(true);
            break;
        }

        case Qt::RightToolBarArea:
        {
            ui->actionToolbarEast->setChecked(true);
            break;
        }

        case Qt::TopToolBarArea:
        {
            ui->actionToolbarNorth->setChecked(true);
            break;
        }

        case Qt::BottomToolBarArea:
        {
            ui->actionToolbarSouth->setChecked(true);
            break;
        }

        default:
        {
            break;
        }
    }

    Qt::ToolBarArea area = config.toolbarArea();
    if (toolBarArea(ui->mainToolBar) != area)
    {
        addToolBar(area, ui->mainToolBar);
    }
}

void MainWindow::setupLanguageActionsMenu()
{
    QActionGroup* actionsGroup{new QActionGroup(this)};
    actionsGroup->addAction(ui->actionLangCSharp);
    actionsGroup->addAction(ui->actionLangC_Cpp);
    actionsGroup->addAction(ui->actionLangJava);
    actionsGroup->addAction(ui->actionLangJavaScript);
    actionsGroup->addAction(ui->actionLangNone);
    actionsGroup->addAction(ui->actionLangObjective_C);
    actionsGroup->addAction(ui->actionLangPHP);
    actionsGroup->addAction(ui->actionLangPython);
    actionsGroup->addAction(ui->actionLangSQL);
    actionsGroup->addAction(ui->actionLangVBasic);

    ui->menuLanguage_mode->addActions(actionsGroup->actions());
}

void MainWindow::showMainPage()
{
    setAvailableFunctionalitiesForMainWindow(true);
    QWidget* fileBrowser{ui->stackedWidget->widget(PAGE_FILE_BROWSER)};
    ui->stackedWidget->setCurrentIndex(PAGE_MAIN);
    if (fileBrowser != nullptr)
    {
        ui->stackedWidget->removeWidget(fileBrowser);

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
        new EditorTabPage(file, config.fontSize(), ui->tabWidget)};

    editorTabPage->setLineWrap(config.lineWrap());

    connect(editorTabPage, &EditorTabPage::redoIsAvailable, this,
            &MainWindow::redoAvailabilityChanged);

    connect(editorTabPage, &EditorTabPage::undoIsAvailable, this,
            &MainWindow::undoAvailabilityChanged);

    connect(editorTabPage, &EditorTabPage::copyCutIsAvailable, this,
            &MainWindow::copyAndCutAvailabilityChanged);

    int newestIndex = ui->tabWidget->addTab(editorTabPage, file->baseName());

    ui->tabWidget->setCurrentIndex(newestIndex);

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
        dynamic_cast<EditorTabPage*>(ui->tabWidget->currentWidget())};

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

        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), newBaseName);
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
    connect(ui->actionOpen_file, &QAction::triggered, this,
            &MainWindow::onActionOpenFileTriggered);
    connect(ui->actionSave_file, &QAction::triggered, this,
            &MainWindow::onActionSaveFileTriggered);
    connect(ui->actionSearch, &QAction::triggered, this,
            &MainWindow::onActionSearchTriggered);
    connect(ui->actionZoom_in, &QAction::triggered, this,
            &MainWindow::onActionZoomInTriggered);
    connect(ui->actionZoom_out, &QAction::triggered, this,
            &MainWindow::onActionZoomOutTriggered);
    connect(ui->actionUndo, &QAction::triggered, this,
            &MainWindow::onActionUndoTriggered);
    connect(ui->actionRedo, &QAction::triggered, this,
            &MainWindow::onActionRedoTriggered);
    connect(ui->actionCopy, &QAction::triggered, this,
            &MainWindow::onActionCopyTriggered);
    connect(ui->actionCut, &QAction::triggered, this,
            &MainWindow::onActionCutTriggered);
    connect(ui->actionPaste, &QAction::triggered, this,
            &MainWindow::onActionPasteTriggered);
    connect(ui->actionTabsWest, &QAction::triggered, this,
            &MainWindow::onActionTabsWestTriggered);
    connect(ui->actionTabsEast, &QAction::triggered, this,
            &MainWindow::onActionTabsEastTriggered);
    connect(ui->actionTabsNorth, &QAction::triggered, this,
            &MainWindow::onActionTabsNorthTriggered);
    connect(ui->actionTabsSouth, &QAction::triggered, this,
            &MainWindow::onActionTabsSouthTriggered);
    connect(ui->actionDecrease50, &QAction::triggered, this,
            &MainWindow::onActionDecrease50Triggered);
    connect(ui->actionDecrease25, &QAction::triggered, this,
            &MainWindow::onActionDecrease25Triggered);
    connect(ui->actionDecrease10, &QAction::triggered, this,
            &MainWindow::onActionDecrease10Triggered);
    connect(ui->actionIncrease10, &QAction::triggered, this,
            &MainWindow::onActionIncrease10Triggered);
    connect(ui->actionIncrease25, &QAction::triggered, this,
            &MainWindow::onActionIncrease25Triggered);
    connect(ui->actionIncrease50, &QAction::triggered, this,
            &MainWindow::onActionIncrease50Triggered);
    connect(ui->actionToolbarFile_Operations, &QAction::triggered, this,
            &MainWindow::onActionToolbarFileOperationsTriggered);
    connect(ui->actionToolbarCut_copy_paste, &QAction::triggered, this,
            &MainWindow::onActionToolbarCutCopyPasteTriggered);
    connect(ui->actionToolbarZoom_in_out, &QAction::triggered, this,
            &MainWindow::onActionToolbarZoomInOutTriggered);
    connect(ui->actionToolbarUndo_redo, &QAction::triggered, this,
            &MainWindow::onActionToolbarUndoRedoTriggered);
    connect(ui->actionToolbarSearch, &QAction::triggered, this,
            &MainWindow::onActionToolbarSearchTriggered);
    connect(ui->actionCheck_spelling_in_comments, &QAction::triggered, this,
            &MainWindow::onActionCheckSpellingInCommentsTriggered);
    connect(ui->actionLine_wrap, &QAction::triggered, this,
            &MainWindow::onActionLineWrapTriggered);
    connect(ui->actionExit, &QAction::triggered, this,
            &MainWindow::onActionExitTriggered);
    connect(ui->actionShowToolbar, &QAction::triggered, this,
            &MainWindow::onActionShowToolbarTriggered);
    connect(ui->actionNew, &QAction::triggered, this,
            &MainWindow::onActionNewTriggered);
    connect(ui->actionSave_as, &QAction::triggered, this,
            &MainWindow::onActionSaveAsTriggered);
    connect(ui->actionLangC_Cpp, &QAction::triggered, this,
            &MainWindow::onActionLangCppCTriggered);
    connect(ui->actionLangJava, &QAction::triggered, this,
            &MainWindow::onActionLangJavaTriggered);
    connect(ui->actionLangObjective_C, &QAction::triggered, this,
            &MainWindow::onActionLangObjectiveCTriggered);
    connect(ui->actionLangCSharp, &QAction::triggered, this,
            &MainWindow::onActionLangCSharpTriggered);
    connect(ui->actionLangPHP, &QAction::triggered, this,
            &MainWindow::onActionLangPHPTriggered);
    connect(ui->actionLangVBasic, &QAction::triggered, this,
            &MainWindow::onActionLangVBasicTriggered);
    connect(ui->actionLangPython, &QAction::triggered, this,
            &MainWindow::onActionLangPythonTriggered);
    connect(ui->actionLangSQL, &QAction::triggered, this,
            &MainWindow::onActionLangSqlTriggered);
    connect(ui->actionLangJavaScript, &QAction::triggered, this,
            &MainWindow::onActionLangJavaScriptTriggered);
    connect(ui->actionLangNone, &QAction::triggered, this,
            &MainWindow::onActionLangNoneTriggered);
    connect(ui->actionAbout, &QAction::triggered, this,
            &MainWindow::onActionAboutTriggered);
    connect(ui->actionToolbarNorth, &QAction::triggered, this,
            &MainWindow::onActionToolbarNorthTriggered);
    connect(ui->actionToolbarSouth, &QAction::triggered, this,
            &MainWindow::onActionToolbarSouthTriggered);
    connect(ui->actionToolbarWest, &QAction::triggered, this,
            &MainWindow::onActionToolbarWestTriggered);
    connect(ui->actionToolbarEast, &QAction::triggered, this,
            &MainWindow::onActionToolbarEastTriggered);
    connect(ui->actionQt_license, &QAction::triggered, this,
            &MainWindow::onActionQtLicenseTriggered);
    connect(ui->actionShow_hide_keyboard, &QAction::triggered, this,
            &MainWindow::onActionShowHideKeyboardTriggered);
    connect(ui->actionToolbarKeyboard, &QAction::triggered, this,
            &MainWindow::onActionToolbarKeyboardTriggered);
}

void MainWindow::manageActions(bool tabExist)
{
    ui->actionSearch->setEnabled(tabExist);
    ui->actionZoom_in->setEnabled(tabExist);
    ui->actionZoom_out->setEnabled(tabExist);
    ui->actionCloseFile->setEnabled(tabExist);
    ui->actionSave_file->setEnabled(tabExist);
    ui->actionSave_as->setEnabled(tabExist);
}

void MainWindow::closeCurrentTab()
{
    int currentIndex{ui->tabWidget->currentIndex()};

    QWidget* tabToDelete{ui->tabWidget->widget(currentIndex)};

    if (tabToDelete == nullptr)
        return;

    QString msg{tr("Close ") + ui->tabWidget->tabText(currentIndex) + "?"};
    QMessageBox::StandardButton answer =
        QMessageBox::question(this, tr("Confirm"), msg);

    if (answer == QMessageBox::Yes)
    {
        ui->tabWidget->removeTab(currentIndex);
        delete tabToDelete;

        if (ui->tabWidget->count() == 0)
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
    ui->mainToolBar->clear();

    ui->mainToolBar->addAction(ui->actionMenu);

    const Config& config{Config::getInstance()};

    if (config.toolbarKeyboardAdded())
        ui->mainToolBar->addAction(ui->actionShow_hide_keyboard);

    if (config.toolbarFileAdded())
    {
        ui->mainToolBar->addAction(ui->actionNew);
        ui->mainToolBar->addAction(ui->actionOpen_file);
        ui->mainToolBar->addAction(ui->actionSave_file);
        ui->mainToolBar->addAction(ui->actionSave_as);
        ui->mainToolBar->addAction(ui->actionCloseFile);
    }

    if (config.toolbarUndoRedoAdded())
    {
        ui->mainToolBar->addAction(ui->actionUndo);
        ui->mainToolBar->addAction(ui->actionRedo);
    }

    if (config.toolbarCopyPasteCutAdded())
    {
        ui->mainToolBar->addAction(ui->actionCopy);
        ui->mainToolBar->addAction(ui->actionCut);
        ui->mainToolBar->addAction(ui->actionPaste);
    }

    if (config.toolbarZoomAdded())
    {
        ui->mainToolBar->addAction(ui->actionZoom_in);
        ui->mainToolBar->addAction(ui->actionZoom_out);
    }

    if (config.toolbarSearchAdded())
    {
        ui->mainToolBar->addAction(ui->actionSearch);
    }
}

void MainWindow::onActionSearchTriggered()
{
    EditorTabPage* currentTab{
        dynamic_cast<EditorTabPage*>(ui->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->flipFindVisibility();
}

void MainWindow::onActionZoomInTriggered()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->zoom(true);
}

void MainWindow::onActionZoomOutTriggered()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->zoom(false);
}

void MainWindow::currentTabPageChanged(int index)
{
    QTabBar* tabBar{ui->tabWidget->findChild<QTabBar*>()};
    tabBar->setVisible(1 < ui->tabWidget->count());

    if (index != -1)
    {
        auto* currentTab{
            dynamic_cast<EditorTabPage*>(ui->tabWidget->currentWidget())};

        if (currentTab != nullptr)
        {
            ui->actionRedo->setEnabled(currentTab->redoAvailable());
            ui->actionUndo->setEnabled(currentTab->undoAvailable());
            ui->actionCopy->setEnabled(!currentTab->selectionEmpty());
            ui->actionCut->setEnabled(!currentTab->selectionEmpty());
            clipboardDataChanged();
            setProperLangActionForMode(currentTab->mode());
            ui->menuLanguage_mode->setEnabled(true);
            return;
        }
    }

    ui->actionRedo->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionPaste->setEnabled(false);
    ui->menuLanguage_mode->setEnabled(false);
}

void MainWindow::redoAvailabilityChanged(bool available)
{
    ui->actionRedo->setEnabled(available);
}

void MainWindow::undoAvailabilityChanged(bool available)
{
    ui->actionUndo->setEnabled(available);
}

void MainWindow::onActionUndoTriggered()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->undo();
}

void MainWindow::onActionRedoTriggered()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->redo();
}

void MainWindow::clipboardDataChanged()
{
    const QClipboard* clipboard{QApplication::clipboard()};

    ui->actionPaste->setEnabled(!clipboard->text().isEmpty());
}

void MainWindow::copyAndCutAvailabilityChanged(bool available)
{
    ui->actionCopy->setEnabled(available);
    ui->actionCut->setEnabled(available);
}

void MainWindow::onActionCopyTriggered()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui->tabWidget->currentWidget())};

    if (currentTab != nullptr)
    {
        currentTab->copy();
        ui->actionPaste->setEnabled(true);
    }
}

void MainWindow::onActionCutTriggered()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui->tabWidget->currentWidget())};

    if (currentTab != nullptr)
    {
        currentTab->cut();
        ui->actionPaste->setEnabled(true);
    }
}

void MainWindow::onActionPasteTriggered()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->paste();
}

void MainWindow::onActionTabsWestTriggered()
{
    changeTabPosition(QTabWidget::West);
}

void MainWindow::onActionTabsEastTriggered()
{
    changeTabPosition(QTabWidget::East);
}

void MainWindow::onActionTabsNorthTriggered()
{
    changeTabPosition(QTabWidget::North);
}

void MainWindow::onActionTabsSouthTriggered()
{
    changeTabPosition(QTabWidget::South);
}

void MainWindow::changeTabPosition(QTabWidget::TabPosition position)
{
    ui->tabWidget->setTabPosition(position);
    Config::getInstance().setTabPosition(position);

    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->refreshVisualIndicators();
}

void MainWindow::onActionOpenFileTriggered()
{
    createAndShowBrowseFilesWidget(true);
}

void MainWindow::onActionSaveAsTriggered()
{
    createAndShowBrowseFilesWidget(false);
}

void MainWindow::createAndShowBrowseFilesWidget(bool openFileMode)
{
    setAvailableFunctionalitiesForMainWindow(false);

    auto* browseFilesWidget{
        new BrowseFilesWidget(openFileMode, ui->stackedWidget)};

    connect(browseFilesWidget, &BrowseFilesWidget::cancelAction, this,
            &MainWindow::showMainPage);

    if (openFileMode)
        connect(browseFilesWidget, &BrowseFilesWidget::filePrepared, this,
                &MainWindow::createNewTab);
    else
        connect(browseFilesWidget, &BrowseFilesWidget::filePrepared, this,
                &MainWindow::saveFileFromTab);

    ui->stackedWidget->insertWidget(PAGE_FILE_BROWSER, browseFilesWidget);

    ui->stackedWidget->setCurrentIndex(PAGE_FILE_BROWSER);
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

    QList<EditorTabPage*> pages{ui->tabWidget->findChildren<EditorTabPage*>()};
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
        dynamic_cast<EditorTabPage*>(ui->tabWidget->currentWidget())};

    if (currentTab != nullptr)
    {
        File* file{currentTab->getCurrentFileCopy()};
        switch (file->source())
        {
            case Common::SOURCE_NOT_SET:
            {
                onActionSaveAsTriggered();
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
    ui->mainToolBar->setVisible(checked);
}

void MainWindow::changeModeForCurrentTab(EditorTabPage::EditorMode mode)
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->setMode(mode);
}

void MainWindow::setProperLangActionForMode(EditorTabPage::EditorMode mode)
{
    switch (mode)
    {
        case EditorTabPage::EDITOR_MODE_C_CPP:
        {
            ui->actionLangC_Cpp->setChecked(true);
            break;
        }

        case EditorTabPage::EDITOR_MODE_JAVA:
        {
            ui->actionLangJava->setChecked(true);
            break;
        }

        case EditorTabPage::EDITOR_MODE_OBJECTIVE_C:
        {
            ui->actionLangObjective_C->setChecked(true);
            break;
        }

        case EditorTabPage::EDITOR_MODE_C_SHARP:
        {
            ui->actionLangCSharp->setChecked(true);
            break;
        }

        case EditorTabPage::EDITOR_MODE_PHP:
        {
            ui->actionLangPHP->setChecked(true);
            break;
        }

        case EditorTabPage::EDITOR_MODE_VISUAL_BASIC:
        {
            ui->actionLangVBasic->setChecked(true);
            break;
        }

        case EditorTabPage::EDITOR_MODE_PYTHON:
        {
            ui->actionLangPython->setChecked(true);
            break;
        }

        case EditorTabPage::EDITOR_MODE_SQL:
        {
            ui->actionLangSQL->setChecked(true);
            break;
        }

        case EditorTabPage::EDITOR_MODE_JAVASCRIPT:
        {
            ui->actionLangJavaScript->setChecked(true);
            break;
        }

        case EditorTabPage::EDITOR_MODE_PLAIN_TEXT:
        {
            ui->actionLangNone->setChecked(true);
            break;
        }

        default:
        {
            break;
        }
    }
}

void MainWindow::onActionLangCppCTriggered()
{
    changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_C_CPP);
}

void MainWindow::onActionLangJavaTriggered()
{
    changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_JAVA);
}

void MainWindow::onActionLangObjectiveCTriggered()
{
    changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_OBJECTIVE_C);
}

void MainWindow::onActionLangCSharpTriggered()
{
    changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_C_SHARP);
}

void MainWindow::onActionLangPHPTriggered()
{
    changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_PHP);
}

void MainWindow::onActionLangVBasicTriggered()
{
    changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_VISUAL_BASIC);
}

void MainWindow::onActionLangPythonTriggered()
{
    changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_PYTHON);
}

void MainWindow::onActionLangSqlTriggered()
{
    changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_SQL);
}

void MainWindow::onActionLangJavaScriptTriggered()
{
    changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_JAVASCRIPT);
}

void MainWindow::onActionLangNoneTriggered()
{
    changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_PLAIN_TEXT);
}

void MainWindow::onActionToolbarNorthTriggered()
{
    Config::getInstance().setToolbarArea(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, ui->mainToolBar);
}

void MainWindow::onActionToolbarSouthTriggered()
{
    Config::getInstance().setToolbarArea(Qt::BottomToolBarArea);
    addToolBar(Qt::BottomToolBarArea, ui->mainToolBar);
}

void MainWindow::onActionToolbarWestTriggered()
{
    Config::getInstance().setToolbarArea(Qt::LeftToolBarArea);
    addToolBar(Qt::LeftToolBarArea, ui->mainToolBar);
}

void MainWindow::onActionToolbarEastTriggered()
{
    Config::getInstance().setToolbarArea(Qt::RightToolBarArea);
    addToolBar(Qt::RightToolBarArea, ui->mainToolBar);
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
    if (ui->stackedWidget->currentIndex() != PAGE_MAIN)
        return;

    QInputMethod* input{QGuiApplication::inputMethod()};
    input->setVisible(!input->isVisible());
}

void MainWindow::setAvailableFunctionalitiesForMainWindow(bool visible)
{
    if (visible)
        ui->mainToolBar->setVisible(ui->actionShowToolbar->isChecked());
    else
        ui->mainToolBar->setVisible(false);

    ui->menuEdit->setEnabled(visible);
    ui->actionShowToolbar->setEnabled(visible);
    ui->menuHelp->setEnabled(visible);
}

void MainWindow::focusHasChanged([[maybe_unused]] QWidget* old, QWidget* now)
{
    auto* codeViewer{dynamic_cast<CodeViewer*>(now)};
    auto* lineEdit{dynamic_cast<QLineEdit*>(now)};
    bool keyboardFocusWidget{lineEdit != nullptr || codeViewer != nullptr};
    ui->actionShow_hide_keyboard->setEnabled(keyboardFocusWidget);
}

void MainWindow::on_actionMenu_triggered()
{
    QMenu menu;
    menu.addMenu(ui->menuFile);
    menu.addMenu(ui->menuEdit);
    menu.addMenu(ui->menuOptions);
    menu.addMenu(ui->menuHelp);
    menu.exec(QCursor::pos());
}
