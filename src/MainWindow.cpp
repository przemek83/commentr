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

    showMainPage();

    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this,
            SLOT(currentTabPageChanged(int)));

    const QClipboard* clipboard = QApplication::clipboard();

    connect(clipboard, SIGNAL(dataChanged()), this,
            SLOT(clipboardDataChanged()));

    initMenus();

    rebuildToolbar();

    Highlighter::setSpellChecking(Config::getInstance().checkSpelling());

    connect(ui->actionCloseFile, SIGNAL(triggered()), this,
            SLOT(closeCurrentTab()));

    connect(qApp, SIGNAL(focusChanged(QWidget*, QWidget*)), this,
            SLOT(focusHasChanged(QWidget*, QWidget*)));
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
            on_actionExit_triggered();

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
        connect(action, SIGNAL(triggered(bool)), this, SLOT(openRecentFile()));
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

        connect(action, SIGNAL(triggered()), this, SLOT(qtStylePicked()));
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

    connect(editorTabPage, SIGNAL(redoAvailable(bool)), this,
            SLOT(redoAvailabilityChanged(bool)));

    connect(editorTabPage, SIGNAL(undoAvailable(bool)), this,
            SLOT(undoAvailabilityChanged(bool)));

    connect(editorTabPage, SIGNAL(copyCutAvailable(bool)), this,
            SLOT(copyAndCutAvailabilityChanged(bool)));

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

void MainWindow::on_actionSearch_triggered()
{
    EditorTabPage* currentTab{
        dynamic_cast<EditorTabPage*>(ui->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->flipFindVisibility();
}

void MainWindow::on_actionZoom_in_triggered()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->zoom(true);
}

void MainWindow::on_actionZoom_out_triggered()
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

void MainWindow::on_actionUndo_triggered()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->undo();
}

void MainWindow::on_actionRedo_triggered()
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

void MainWindow::on_actionCopy_triggered()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui->tabWidget->currentWidget())};

    if (currentTab != nullptr)
    {
        currentTab->copy();
        ui->actionPaste->setEnabled(true);
    }
}

void MainWindow::on_actionCut_triggered()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui->tabWidget->currentWidget())};

    if (currentTab != nullptr)
    {
        currentTab->cut();
        ui->actionPaste->setEnabled(true);
    }
}

void MainWindow::on_actionPaste_triggered()
{
    auto* currentTab{
        dynamic_cast<EditorTabPage*>(ui->tabWidget->currentWidget())};

    if (currentTab != nullptr)
        currentTab->paste();
}

void MainWindow::on_actionTabsWest_triggered()
{
    changeTabPosition(QTabWidget::West);
}

void MainWindow::on_actionTabsEast_triggered()
{
    changeTabPosition(QTabWidget::East);
}

void MainWindow::on_actionTabsNorth_triggered()
{
    changeTabPosition(QTabWidget::North);
}

void MainWindow::on_actionTabsSouth_triggered()
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

void MainWindow::on_actionOpen_file_triggered()
{
    createAndShowBrowseFilesWidget(true);
}

void MainWindow::on_actionSave_as_triggered()
{
    createAndShowBrowseFilesWidget(false);
}

void MainWindow::createAndShowBrowseFilesWidget(bool openFileMode)
{
    setAvailableFunctionalitiesForMainWindow(false);

    auto* browseFilesWidget{
        new BrowseFilesWidget(openFileMode, ui->stackedWidget)};

    connect(browseFilesWidget, SIGNAL(cancelAction()), this,
            SLOT(showMainPage()));

    if (openFileMode)
        connect(browseFilesWidget, SIGNAL(filePrepared(File*)), this,
                SLOT(createNewTab(File*)));
    else
        connect(browseFilesWidget, SIGNAL(filePrepared(File*)), this,
                SLOT(saveFileFromTab(File*)));

    ui->stackedWidget->insertWidget(PAGE_FILE_BROWSER, browseFilesWidget);

    ui->stackedWidget->setCurrentIndex(PAGE_FILE_BROWSER);
}

void MainWindow::on_actionDecrease50_triggered()
{
    float currentSize = Config::getInstance().uiSize();
    Config::getInstance().setUiSize(currentSize * .5);
    ProxyStyle::updateUisize();
}

void MainWindow::on_actionDecrease25_triggered()
{
    float currentSize = Config::getInstance().uiSize();
    Config::getInstance().setUiSize(currentSize * .75);
    ProxyStyle::updateUisize();
}

void MainWindow::on_actionDecrease10_triggered()
{
    float currentSize = Config::getInstance().uiSize();
    Config::getInstance().setUiSize(currentSize * .90);
    ProxyStyle::updateUisize();
}

void MainWindow::on_actionIncrease10_triggered()
{
    float currentSize = Config::getInstance().uiSize();
    Config::getInstance().setUiSize(currentSize * 1.1);
    ProxyStyle::updateUisize();
}

void MainWindow::on_actionIncrease25_triggered()
{
    float currentSize = Config::getInstance().uiSize();
    Config::getInstance().setUiSize(currentSize * 1.25);
    ProxyStyle::updateUisize();
}

void MainWindow::on_actionIncrease50_triggered()
{
    float currentSize = Config::getInstance().uiSize();
    Config::getInstance().setUiSize(currentSize * 1.5);
    ProxyStyle::updateUisize();
}

void MainWindow::on_actionToolbarKeyboard_triggered(bool checked)
{
    Config::getInstance().setToolbarKeyboardAdded(checked);
    rebuildToolbar();
}

void MainWindow::on_actionToolbarFile_Operations_triggered(bool checked)
{
    Config::getInstance().setToolbarFileAdded(checked);
    rebuildToolbar();
}

void MainWindow::on_actionToolbarCut_copy_paste_triggered(bool checked)
{
    Config::getInstance().setToolbarCopyPasteCutAdded(checked);
    rebuildToolbar();
}

void MainWindow::on_actionToolbarZoom_in_out_triggered(bool checked)
{
    Config::getInstance().setToolbarZoomAdded(checked);
    rebuildToolbar();
}

void MainWindow::on_actionToolbarUndo_redo_triggered(bool checked)
{
    Config::getInstance().setToolbarUndoRedoAdded(checked);
    rebuildToolbar();
}

void MainWindow::on_actionToolbarSearch_triggered(bool checked)
{
    Config::getInstance().setToolbarSearchAdded(checked);
    rebuildToolbar();
}

void MainWindow::on_actionCheck_spelling_in_comments_triggered(bool checked)
{
    Config::getInstance().setCheckSpelling(checked);

    Highlighter::setSpellChecking(checked);

    QList<Highlighter*> highlighters{findChildren<Highlighter*>()};
    foreach (Highlighter* highlighter, highlighters)
        highlighter->rehighlight();
}

void MainWindow::on_actionLine_wrap_triggered(bool checked)
{
    Config::getInstance().setLineWrap(checked);

    QList<EditorTabPage*> pages{ui->tabWidget->findChildren<EditorTabPage*>()};
    foreach (EditorTabPage* page, pages)
        page->setLineWrap(checked);
}

void MainWindow::on_actionExit_triggered()
{
    QMessageBox::StandardButton answer{
        QMessageBox::question(this, tr("Quit"), tr("Quit CommentR?"))};

    if (answer == QMessageBox::No)
        return;

    QCoreApplication::quit();
}

void MainWindow::on_actionSave_file_triggered()
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
                on_actionSave_as_triggered();
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

void MainWindow::on_actionShowToolbar_triggered(bool checked)
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

void MainWindow::on_actionLangC_Cpp_triggered()
{
    changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_C_CPP);
}

void MainWindow::on_actionLangJava_triggered()
{
    changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_JAVA);
}

void MainWindow::on_actionLangObjective_C_triggered()
{
    changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_OBJECTIVE_C);
}

void MainWindow::on_actionLangCSharp_triggered()
{
    changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_C_SHARP);
}

void MainWindow::on_actionLangPHP_triggered()
{
    changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_PHP);
}

void MainWindow::on_actionLangVBasic_triggered()
{
    changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_VISUAL_BASIC);
}

void MainWindow::on_actionLangPython_triggered()
{
    changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_PYTHON);
}

void MainWindow::on_actionLangSQL_triggered()
{
    changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_SQL);
}

void MainWindow::on_actionLangJavaScript_triggered()
{
    changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_JAVASCRIPT);
}

void MainWindow::on_actionLangNone_triggered()
{
    changeModeForCurrentTab(EditorTabPage::EDITOR_MODE_PLAIN_TEXT);
}

void MainWindow::on_actionToolbarNorth_triggered()
{
    Config::getInstance().setToolbarArea(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, ui->mainToolBar);
}

void MainWindow::on_actionToolbarSouth_triggered()
{
    Config::getInstance().setToolbarArea(Qt::BottomToolBarArea);
    addToolBar(Qt::BottomToolBarArea, ui->mainToolBar);
}

void MainWindow::on_actionToolbarWest_triggered()
{
    Config::getInstance().setToolbarArea(Qt::LeftToolBarArea);
    addToolBar(Qt::LeftToolBarArea, ui->mainToolBar);
}

void MainWindow::on_actionToolbarEast_triggered()
{
    Config::getInstance().setToolbarArea(Qt::RightToolBarArea);
    addToolBar(Qt::RightToolBarArea, ui->mainToolBar);
}

void MainWindow::on_actionNew_triggered()
{
    showMainPage();
    ++newFileCounter_;
    File* file{new File(Common::SOURCE_NOT_SET, "",
                        tr("File") + QString::number(newFileCounter_), "",
                        new QString(""))};

    createNewTab(file);
}

void MainWindow::on_actionAbout_triggered()
{
    File* file{new File(Common::SOURCE_NOT_SET, "",
                        "About " + QCoreApplication::applicationName(), "",
                        new QString(Common::loadFile(":/about.txt")))};

    createNewTab(file);
}

void MainWindow::on_actionQt_license_triggered()
{
    File* file{new File(Common::SOURCE_NOT_SET, "", "Qt license", "",
                        new QString(Common::loadFile(":/LICENSE")))};

    createNewTab(file);
}

void MainWindow::on_actionShow_hide_keyboard_triggered()
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
