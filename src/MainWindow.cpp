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
#include "SpellChecker.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(Config config, SpellChecker spellChecker,
                       QWidget* parent)
    : QMainWindow(parent),
      ui_{std::make_unique<Ui::MainWindow>()},
      config_{std::move(config)},
      spellChecker_{std::move(spellChecker)}
{
    QApplication::setStyle(new ProxyStyle(config_.style(), config_.uiSize()));

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

    spellChecker_.setActive(config_.checkSpelling());

    connect(ui_->actionCloseFile, &QAction::triggered, this,
            &MainWindow::closeCurrentTab);

    connect(qApp, &QApplication::focusChanged, this,
            &MainWindow::focusHasChanged);
}

MainWindow::~MainWindow() { config_.save(); };

void MainWindow::keyReleaseEvent(QKeyEvent* e)
{
    const int key{e->key()};

    if (Qt::Key_Back == key)
    {
        auto page{static_cast<StackedPage>(ui_->stackedWidget->currentIndex())};
        if (PAGE_MAIN != page)
            showMainPage();
        else
            exitApplication();

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
    setupStyles(config_);

    setupTabsAlignmentMenu(config_);

    setupToolbarActionsMenu(config_);

    setupLanguageActionsMenu();

    setupRecentFiles(config_);

    ui_->actionCheck_spelling_in_comments->setChecked(config_.checkSpelling());
    ui_->actionLine_wrap->setChecked(config_.lineWrap());
    ui_->mainToolBar->setVisible(config_.showToolbar());
    ui_->actionShowToolbar->setChecked(config_.showToolbar());
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

    switch (config_.toolbarArea())
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
    setAvailableFunctionalities(true);
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
    showMainPage();

    EditorTabPage* editorTabPage{new EditorTabPage(
        file, config_.fontSize(), config_, spellChecker_, ui_->tabWidget)};

    editorTabPage->setLineWrap(config_.lineWrap());

    connect(editorTabPage, &EditorTabPage::redoIsAvailable,
            [this](bool available) { ui_->actionRedo->setEnabled(available); });

    connect(editorTabPage, &EditorTabPage::undoIsAvailable,
            [this](bool available) { ui_->actionUndo->setEnabled(available); });

    connect(editorTabPage, &EditorTabPage::copyCutIsAvailable, this,
            &MainWindow::copyAndCutAvailabilityChanged);

    int newestIndex = ui_->tabWidget->addTab(editorTabPage, file->baseName());

    ui_->tabWidget->setCurrentIndex(newestIndex);

    manageActions(true);

    if (file->source() == Common::Source::LOCAL)
    {
        config_.addFilePathToRecentFiles(file->getFilePath());

        setupRecentFiles(config_);
    }
}

void MainWindow::saveFileFromTab(File* file)
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab == nullptr)
    {
        showMainPage();
        return;
    }

    QString newBaseName = file->baseName();
    if (file->source() == Common::Source::LOCAL)
    {
        QString content(currentTab->getCurrentText());
        showStatusMsg(Common::saveFile(file->getFilePath(), content));
        currentTab->changeFile(file);
    }
    delete file;
    ui_->tabWidget->setTabText(ui_->tabWidget->currentIndex(), newBaseName);
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

        File* file{new File(Common::Source::LOCAL,
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
            &MainWindow::saveFile);
    connect(ui_->actionSearch, &QAction::triggered, this, &MainWindow::search);
    connect(ui_->actionZoom_in, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(ui_->actionZoom_out, &QAction::triggered, this,
            &MainWindow::zoomOut);
    connect(ui_->actionUndo, &QAction::triggered, this, &MainWindow::undo);
    connect(ui_->actionRedo, &QAction::triggered, this, &MainWindow::redo);
    connect(ui_->actionCopy, &QAction::triggered, this, &MainWindow::copy);
    connect(ui_->actionCut, &QAction::triggered, this, &MainWindow::cut);
    connect(ui_->actionPaste, &QAction::triggered, this, &MainWindow::paste);

    connect(ui_->actionTabsWest, &QAction::triggered,
            [this]() { changeTabPosition(QTabWidget::West); });
    connect(ui_->actionTabsEast, &QAction::triggered,
            [this]() { changeTabPosition(QTabWidget::East); });
    connect(ui_->actionTabsNorth, &QAction::triggered,
            [this]() { changeTabPosition(QTabWidget::North); });
    connect(ui_->actionTabsSouth, &QAction::triggered,
            [this]() { changeTabPosition(QTabWidget::South); });

    setupChangeSizeActions();

    connect(ui_->actionToolbarFile_Operations, &QAction::triggered, this,
            &MainWindow::toolbarFileOperations);
    connect(ui_->actionToolbarCut_copy_paste, &QAction::triggered, this,
            &MainWindow::toolbarCutCopyPaste);
    connect(ui_->actionToolbarZoom_in_out, &QAction::triggered, this,
            &MainWindow::toolbarZoomInOut);
    connect(ui_->actionToolbarUndo_redo, &QAction::triggered, this,
            &MainWindow::toolbarUndoRedo);
    connect(ui_->actionToolbarSearch, &QAction::triggered, this,
            &MainWindow::toolbarSearch);
    connect(ui_->actionCheck_spelling_in_comments, &QAction::triggered, this,
            &MainWindow::checkSpellingInComments);
    connect(ui_->actionLine_wrap, &QAction::triggered, this,
            &MainWindow::lineWrap);
    connect(ui_->actionExit, &QAction::triggered, this,
            &MainWindow::exitApplication);
    connect(ui_->actionShowToolbar, &QAction::triggered, this,
            &MainWindow::showToolbar);
    connect(ui_->actionNew, &QAction::triggered, this, &MainWindow::newFile);
    connect(ui_->actionSave_as, &QAction::triggered,
            [this]() { createAndShowBrowseFilesWidget(false); });

    setupEditorModeActions();

    connect(ui_->actionAbout, &QAction::triggered, this,
            &MainWindow::showAbout);

    setupToolbarPositionActions();

    connect(ui_->actionQt_license, &QAction::triggered, this,
            &MainWindow::showQtLicense);
    connect(ui_->actionShow_hide_keyboard, &QAction::triggered, this,
            &MainWindow::showHidenKeyboard);
    connect(ui_->actionToolbarKeyboard, &QAction::triggered, this,
            &MainWindow::toolbarKeyboardActivated);
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
        QWidget* focusWidget{QApplication::focusWidget()};
        if (focusWidget != nullptr)
            focusWidget->clearFocus();

        config_.setStyle(style);
        qApp->setStyleSheet(QString());
        QApplication::setStyle(new ProxyStyle(style, config_.uiSize()));
    }
}

void MainWindow::rebuildToolbar()
{
    ui_->mainToolBar->clear();

    ui_->mainToolBar->addAction(ui_->actionMenu);

    if (config_.toolbarKeyboardAdded())
        ui_->mainToolBar->addAction(ui_->actionShow_hide_keyboard);

    if (config_.toolbarFileAdded())
    {
        ui_->mainToolBar->addAction(ui_->actionNew);
        ui_->mainToolBar->addAction(ui_->actionOpen_file);
        ui_->mainToolBar->addAction(ui_->actionSave_file);
        ui_->mainToolBar->addAction(ui_->actionSave_as);
        ui_->mainToolBar->addAction(ui_->actionCloseFile);
    }

    if (config_.toolbarUndoRedoAdded())
    {
        ui_->mainToolBar->addAction(ui_->actionUndo);
        ui_->mainToolBar->addAction(ui_->actionRedo);
    }

    if (config_.toolbarCopyPasteCutAdded())
    {
        ui_->mainToolBar->addAction(ui_->actionCopy);
        ui_->mainToolBar->addAction(ui_->actionCut);
        ui_->mainToolBar->addAction(ui_->actionPaste);
    }

    if (config_.toolbarZoomAdded())
    {
        ui_->mainToolBar->addAction(ui_->actionZoom_in);
        ui_->mainToolBar->addAction(ui_->actionZoom_out);
    }

    if (config_.toolbarSearchAdded())
    {
        ui_->mainToolBar->addAction(ui_->actionSearch);
    }
}

void MainWindow::search()
{
    EditorTabPage* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->flipFindVisibility();
}

void MainWindow::zoomIn()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->zoom(true);
}

void MainWindow::zoomOut()
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

void MainWindow::undo()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->undo();
}

void MainWindow::redo()
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

void MainWindow::copy()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab != nullptr)
    {
        currentTab->copy();
        ui_->actionPaste->setEnabled(true);
    }
}

void MainWindow::cut()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab != nullptr)
    {
        currentTab->cut();
        ui_->actionPaste->setEnabled(true);
    }
}

void MainWindow::paste()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->paste();
}

void MainWindow::changeTabPosition(QTabWidget::TabPosition position)
{
    ui_->tabWidget->setTabPosition(position);
    config_.setTabPosition(position);

    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->refreshVisualIndicators();
}

void MainWindow::createAndShowBrowseFilesWidget(bool openFileMode)
{
    setAvailableFunctionalities(false);

    auto* browseFilesWidget{
        new BrowseFilesWidget(openFileMode, config_, ui_->stackedWidget)};

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

void MainWindow::changeSize(float factor)
{
    const float currentSize = config_.uiSize();
    const float newSize = currentSize * factor;
    config_.setUiSize(newSize);
    QApplication::setStyle(new ProxyStyle(config_.style(), newSize));
}

void MainWindow::changeToolbarPosition(Qt::ToolBarArea area)
{
    config_.setToolbarArea(area);
    addToolBar(area, ui_->mainToolBar);
}

void MainWindow::setupChangeSizeActions()
{
    constexpr float decreaseSizeBy50Percent{0.5F};
    constexpr float decreaseSizeBy25Percent{0.75F};
    constexpr float decreaseSizeBy10Percent{0.90F};
    constexpr float increaseSizeBy10Percent{1.1F};
    constexpr float increaseSizeBy25Percent{1.25F};
    constexpr float increaseSizeBy50Percent{1.5F};

    connect(ui_->actionDecrease50, &QAction::triggered,
            [this, decreaseSizeBy50Percent]()
            { changeSize(decreaseSizeBy50Percent); });
    connect(ui_->actionDecrease25, &QAction::triggered,
            [this, decreaseSizeBy25Percent]()
            { changeSize(decreaseSizeBy25Percent); });
    connect(ui_->actionDecrease10, &QAction::triggered,
            [this, decreaseSizeBy10Percent]()
            { changeSize(decreaseSizeBy10Percent); });
    connect(ui_->actionIncrease10, &QAction::triggered,
            [this, increaseSizeBy10Percent]()
            { changeSize(increaseSizeBy10Percent); });
    connect(ui_->actionIncrease25, &QAction::triggered,
            [this, increaseSizeBy25Percent]()
            { changeSize(increaseSizeBy25Percent); });
    connect(ui_->actionIncrease50, &QAction::triggered,
            [this, increaseSizeBy50Percent]()
            { changeSize(increaseSizeBy50Percent); });
}

void MainWindow::setupToolbarPositionActions()
{
    connect(ui_->actionToolbarNorth, &QAction::triggered,
            [this]() { changeToolbarPosition(Qt::TopToolBarArea); });
    connect(ui_->actionToolbarSouth, &QAction::triggered,
            [this]() { changeToolbarPosition(Qt::BottomToolBarArea); });
    connect(ui_->actionToolbarWest, &QAction::triggered,
            [this]() { changeToolbarPosition(Qt::LeftToolBarArea); });
    connect(ui_->actionToolbarEast, &QAction::triggered,
            [this]() { changeToolbarPosition(Qt::RightToolBarArea); });
}

void MainWindow::setupEditorModeActions()
{
    connect(ui_->actionLangC_Cpp, &QAction::triggered, [this]()
            { changeModeForCurrentTab(EditorTabPage::EditorMode::C_CPP); });
    connect(ui_->actionLangJava, &QAction::triggered, [this]()
            { changeModeForCurrentTab(EditorTabPage::EditorMode::JAVA); });
    connect(
        ui_->actionLangObjective_C, &QAction::triggered, [this]()
        { changeModeForCurrentTab(EditorTabPage::EditorMode::OBJECTIVE_C); });
    connect(ui_->actionLangCSharp, &QAction::triggered, [this]()
            { changeModeForCurrentTab(EditorTabPage::EditorMode::C_SHARP); });
    connect(ui_->actionLangPHP, &QAction::triggered, [this]()
            { changeModeForCurrentTab(EditorTabPage::EditorMode::PHP); });
    connect(
        ui_->actionLangVBasic, &QAction::triggered, [this]()
        { changeModeForCurrentTab(EditorTabPage::EditorMode::VISUAL_BASIC); });
    connect(ui_->actionLangPython, &QAction::triggered, [this]()
            { changeModeForCurrentTab(EditorTabPage::EditorMode::PYTHON); });
    connect(ui_->actionLangSQL, &QAction::triggered, [this]()
            { changeModeForCurrentTab(EditorTabPage::EditorMode::SQL); });
    connect(
        ui_->actionLangJavaScript, &QAction::triggered, [this]()
        { changeModeForCurrentTab(EditorTabPage::EditorMode::JAVASCRIPT); });
    connect(
        ui_->actionLangNone, &QAction::triggered, [this]()
        { changeModeForCurrentTab(EditorTabPage::EditorMode::PLAIN_TEXT); });
}

void MainWindow::toolbarKeyboardActivated(bool checked)
{
    config_.setToolbarKeyboardAdded(checked);
    rebuildToolbar();
}

void MainWindow::toolbarFileOperations(bool checked)
{
    config_.setToolbarFileAdded(checked);
    rebuildToolbar();
}

void MainWindow::toolbarCutCopyPaste(bool checked)
{
    config_.setToolbarCopyPasteCutAdded(checked);
    rebuildToolbar();
}

void MainWindow::toolbarZoomInOut(bool checked)
{
    config_.setToolbarZoomAdded(checked);
    rebuildToolbar();
}

void MainWindow::toolbarUndoRedo(bool checked)
{
    config_.setToolbarUndoRedoAdded(checked);
    rebuildToolbar();
}

void MainWindow::toolbarSearch(bool checked)
{
    config_.setToolbarSearchAdded(checked);
    rebuildToolbar();
}

void MainWindow::checkSpellingInComments(bool checked)
{
    config_.setCheckSpelling(checked);

    spellChecker_.setActive(checked);

    QList<EditorTabPage*> pages{findChildren<EditorTabPage*>()};
    foreach (EditorTabPage* page, pages)
        page->refreshHighlighter();
}

void MainWindow::lineWrap(bool checked)
{
    config_.setLineWrap(checked);

    QList<EditorTabPage*> pages{ui_->tabWidget->findChildren<EditorTabPage*>()};
    foreach (EditorTabPage* page, pages)
        page->setLineWrap(checked);
}

void MainWindow::exitApplication()
{
    QMessageBox::StandardButton answer{
        QMessageBox::question(this, tr("Quit"), tr("Quit CommentR?"))};

    if (answer == QMessageBox::No)
        return;

    QCoreApplication::quit();
}

void MainWindow::saveFile()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui_->tabWidget->currentWidget())};

    if (currentTab != nullptr)
    {
        File* file{currentTab->getCurrentFileCopy()};
        switch (file->source())
        {
            case Common::Source::NOT_SET:
            {
                createAndShowBrowseFilesWidget(false);
                break;
            }

            case Common::Source::LOCAL:
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

void MainWindow::showToolbar(bool checked)
{
    config_.setShowToolbar(checked);
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
        case EditorTabPage::EditorMode::C_CPP:
        {
            ui_->actionLangC_Cpp->setChecked(true);
            break;
        }

        case EditorTabPage::EditorMode::JAVA:
        {
            ui_->actionLangJava->setChecked(true);
            break;
        }

        case EditorTabPage::EditorMode::OBJECTIVE_C:
        {
            ui_->actionLangObjective_C->setChecked(true);
            break;
        }

        case EditorTabPage::EditorMode::C_SHARP:
        {
            ui_->actionLangCSharp->setChecked(true);
            break;
        }

        case EditorTabPage::EditorMode::PHP:
        {
            ui_->actionLangPHP->setChecked(true);
            break;
        }

        case EditorTabPage::EditorMode::VISUAL_BASIC:
        {
            ui_->actionLangVBasic->setChecked(true);
            break;
        }

        case EditorTabPage::EditorMode::PYTHON:
        {
            ui_->actionLangPython->setChecked(true);
            break;
        }

        case EditorTabPage::EditorMode::SQL:
        {
            ui_->actionLangSQL->setChecked(true);
            break;
        }

        case EditorTabPage::EditorMode::JAVASCRIPT:
        {
            ui_->actionLangJavaScript->setChecked(true);
            break;
        }

        case EditorTabPage::EditorMode::PLAIN_TEXT:
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

void MainWindow::newFile()
{
    showMainPage();
    ++newFileCounter_;
    File* file{new File(Common::Source::NOT_SET, "",
                        tr("File") + QString::number(newFileCounter_), "",
                        new QString(""))};

    createNewTab(file);
}

void MainWindow::showAbout()
{
    File* file{new File(Common::Source::NOT_SET, "",
                        "About " + QCoreApplication::applicationName(), "",
                        new QString(Common::loadFile(":/about.txt")))};

    createNewTab(file);
}

void MainWindow::showQtLicense()
{
    File* file{new File(Common::Source::NOT_SET, "", "Qt license", "",
                        new QString(Common::loadFile(":/LICENSE")))};

    createNewTab(file);
}

void MainWindow::showHidenKeyboard()
{
    if (ui_->stackedWidget->currentIndex() != PAGE_MAIN)
        return;

    QInputMethod* input{QGuiApplication::inputMethod()};
    input->setVisible(!input->isVisible());
}

void MainWindow::setAvailableFunctionalities(bool visible)
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
