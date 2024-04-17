#include <QString>
#include <QtXml/QDomDocument>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QApplication>
#include <QSettings>
#include <QDesktopWidget>

#include "Config.h"
#include "Common.h"

const char* Config::configNames_[] =
{
    "tabsPosition",
    "uiSize",
    "style",
    "toolbar/file",
    "toolbar/undoRedo",
    "toolbar/copyPasteCut",
    "toolbar/zoom",
    "toolbar/search",
    "toolbar/keyboard",
    "checkSpelling",
    "lineWrap",
    "firstUse",
    "showtoolbar",
    "toolbarposition",
    "fontsize",
    "listViewInBrowser",
    "keyboardAfterTap",
    "lastdir",
    "ftpHost",
    "ftpLogin",
    "ftpPassword",
    "saveftppassword",
    "recentFileList"
};

Config::Config() :
    QObject()
{
    load();
}

Config::~Config()
{

}

Config& Config::getInstance()
{
    static Config instance;
    return instance;
}

void Config::save()
{
    QSettings settings;
    setValue(settings, CONFIG_TABS_POSITION, static_cast<int>(tabsPosition_));
    setValue(settings, CONFIG_UI_SIZE, uiSize_);
    setValue(settings, CONFIG_STYLE, style_);
    setValue(settings, CONFIG_TOOLBAR_FILE, toolbarFileAdded_);
    setValue(settings, CONFIG_TOOLBAR_UNDO_REDO, toolbarUndoRedoAdded_);
    setValue(settings, CONFIG_TOOLBAR_COPY_PASTE_CUT, toolbarCopyPasteCutAdded_);
    setValue(settings, CONFIG_TOOLBAR_ZOOM, toolbarZoomAdded_);
    setValue(settings, CONFIG_TOOLBAR_SEARCH, toolbarSearchAdded_);
    setValue(settings, CONFIG_TOOLBAR_KEYBOARD, toolbarKeyboardAdded_);
    setValue(settings, CONFIG_CHECK_SPELLING, checkSpelling_);
    setValue(settings, CONFIG_LINE_WRAP, lineWrap_);
    setValue(settings, CONFIG_FIRST_USE, false);
    setValue(settings, CONFIG_SHOW_TOOLBAR, showToolbar_);
    setValue(settings, CONFIG_TOOLBAR_POSITION, static_cast<int>(toolbarArea_));
    setValue(settings, CONFIG_FONT_SIZE, fontSize_);
    setValue(settings, CONFIG_LIST_VIEW_IN_BROWSER, listViewInBrowser_);
    setValue(settings, CONFIG_KEYBOARD_AFTER_TAP, keyboardAfterTap_);
    setValue(settings, CONFIG_LAST_DIR, lastPickedDir_);
    setValue(settings, CONFIG_FTP_HOST, ftpHost_);
    setValue(settings, CONFIG_FTP_LOGIN, ftpLogin_);
    setValue(settings, CONFIG_FTP_PASSWORD, ftpPassword_);
    setValue(settings, CONFIG_SAVE_FTP_PASSWORD, saveFtpPassword_);
    setValue(settings, CONFIG_RECENT_FILES, recentFiles_);

    settings.sync();
}

void Config::setValue(QSettings& settings, ConfigNames name, QVariant value)
{
    settings.setValue(configNames_[name], value);
}

void Config::load()
{
    QSettings settings;

    QString fieldName = configNames_[CONFIG_TABS_POSITION];
    int tabPosition = settings.value(fieldName).toInt();
    tabsPosition_ = static_cast<QTabWidget::TabPosition>(tabPosition);

    fieldName = configNames_[CONFIG_UI_SIZE];
    uiSize_ = settings.value(fieldName, QVariant(0)).toInt();

    fieldName = configNames_[CONFIG_STYLE];
    style_ = settings.value(fieldName, QVariant("Fusion")).toString();

    fieldName = configNames_[CONFIG_TOOLBAR_FILE];
    toolbarFileAdded_ = settings.value(fieldName, QVariant(true)).toBool();

    fieldName = configNames_[CONFIG_TOOLBAR_UNDO_REDO];
    toolbarUndoRedoAdded_ = settings.value(fieldName, QVariant(true)).toBool();

    fieldName = configNames_[CONFIG_TOOLBAR_COPY_PASTE_CUT];
    toolbarCopyPasteCutAdded_ = settings.value(fieldName, QVariant(true)).toBool();

    fieldName = configNames_[CONFIG_TOOLBAR_ZOOM];
    toolbarZoomAdded_ = settings.value(fieldName, QVariant(false)).toBool();

    fieldName = configNames_[CONFIG_TOOLBAR_SEARCH];
    toolbarSearchAdded_ = settings.value(fieldName, QVariant(true)).toBool();

    fieldName = configNames_[CONFIG_TOOLBAR_KEYBOARD];
    toolbarKeyboardAdded_ = settings.value(fieldName, QVariant(true)).toBool();

    fieldName = configNames_[CONFIG_CHECK_SPELLING];
    checkSpelling_ = settings.value(fieldName, QVariant(true)).toBool();

    fieldName = configNames_[CONFIG_LINE_WRAP];
    lineWrap_ = settings.value(fieldName, QVariant(true)).toBool();

    fieldName = configNames_[CONFIG_FIRST_USE];
    firstUse_ = settings.value(fieldName, QVariant(true)).toBool();

    fieldName = configNames_[CONFIG_SHOW_TOOLBAR];
    showToolbar_ = settings.value(fieldName, QVariant(true)).toBool();

    fieldName = configNames_[CONFIG_TOOLBAR_POSITION];
    int toolbarPosition =
        settings.value(fieldName,
                       QVariant(static_cast<int>(Qt::TopToolBarArea))).toInt();
    toolbarArea_ = static_cast<Qt::ToolBarArea>(toolbarPosition);

    fieldName = configNames_[CONFIG_FONT_SIZE];
    setFontSize(settings.value(fieldName, QVariant(10)).toFloat());

    fieldName = configNames_[CONFIG_LIST_VIEW_IN_BROWSER];
    listViewInBrowser_ = settings.value(fieldName, QVariant(true)).toBool();

    fieldName = configNames_[CONFIG_KEYBOARD_AFTER_TAP];
    keyboardAfterTap_ = settings.value(fieldName, QVariant(true)).toBool();

    fieldName = configNames_[CONFIG_LAST_DIR];
    lastPickedDir_ = settings.value(fieldName, QVariant("/")).toString();

    fieldName = configNames_[CONFIG_FTP_HOST];
    ftpHost_ = settings.value(fieldName, QVariant("")).toString();

    fieldName = configNames_[CONFIG_FTP_LOGIN];
    ftpLogin_ = settings.value(fieldName, QVariant("")).toString();

    fieldName = configNames_[CONFIG_FTP_PASSWORD];
    ftpPassword_ = settings.value(fieldName, QVariant("")).toString();

    fieldName = configNames_[CONFIG_SAVE_FTP_PASSWORD];
    saveFtpPassword_ = settings.value(fieldName, QVariant(false)).toBool();

    fieldName = configNames_[CONFIG_RECENT_FILES];
    recentFiles_ = settings.value(fieldName, QVariant(QStringList())).toStringList();
}

const QStringList& Config::getRecentFiles() const
{
    return recentFiles_;
}

void Config::addFilePathToRecentFiles(const QString& filePath)
{
    static const int maxRecentFiles = Common::getMaxRecentFiles();
    if (recentFiles_.contains(filePath) == false)
    {
        if(recentFiles_.size() >= maxRecentFiles)
        {
            recentFiles_.removeLast();
        }
        recentFiles_.prepend(filePath);
    }
}

bool Config::saveFtpPassword() const
{
    return saveFtpPassword_;
}

void Config::setSaveFtpPassword(bool saveFtpPassword)
{
    saveFtpPassword_ = saveFtpPassword;
}


QString Config::ftpHost() const
{
    return ftpHost_;//"mirror.one.com";
}

void Config::setFtpHost(const QString& ftpHost)
{
    ftpHost_ = ftpHost;
}

QString Config::ftpPassword() const
{
    return ftpPassword_;
}

void Config::setFtpPassword(const QString &ftpPassword)
{
    ftpPassword_ = ftpPassword;
}

QString Config::ftpLogin() const
{
    return ftpLogin_;//"anonymous";
}

void Config::setFtpLogin(const QString &ftpLogin)
{
    ftpLogin_ = ftpLogin;
}

QString Config::lastPickedDir() const
{
    return lastPickedDir_;
}

void Config::setLastPickedDir(const QString &lastPickedDir)
{
    lastPickedDir_ = lastPickedDir;
}

bool Config::keyboardAfterTap() const
{
    return keyboardAfterTap_;
}

void Config::setKeyboardAfterTap(bool keyboardAfterTap)
{
    keyboardAfterTap_ = keyboardAfterTap;
}

bool Config::toolbarKeyboardAdded() const
{
    return toolbarKeyboardAdded_;
}

void Config::setToolbarKeyboardAdded(bool toolbarKeyboardAdded)
{
    toolbarKeyboardAdded_ = toolbarKeyboardAdded;
}


bool Config::listViewInBrowser() const
{
    return listViewInBrowser_;
}

void Config::setListViewInBrowser(bool listViewInBrowser)
{
    listViewInBrowser_ = listViewInBrowser;
}

void Config::setDefaultFont()
{
    fontSize_ = QApplication::font().pointSizeF();
}

void Config::determineUiSize()
{
    int biggerSize =
            qMax(QApplication::desktop()->screenGeometry().width(),
                 QApplication::desktop()->screenGeometry().height());

    //Aim in 3/4 of wider screen dimension for 15 icons.
    uiSize_ = biggerSize * 3 / 4 / 15;

    return;
}

float Config::fontSize() const
{
    return fontSize_;
}

void Config::setFontSize(float fontSize)
{
    fontSize_ = Common::normalizeFont(fontSize);
}

Qt::ToolBarArea Config::toolbarArea() const
{
    return toolbarArea_;
}

void Config::setToolbarArea(const Qt::ToolBarArea &toolbarArea)
{
    toolbarArea_ = toolbarArea;
}

bool Config::showToolbar() const
{
    return showToolbar_;
}

void Config::setShowToolbar(bool showToolbar)
{
    showToolbar_ = showToolbar;
}

bool Config::firstUse() const
{
    return firstUse_;
}

void Config::setFirstUse(bool firstUse)
{
    firstUse_ = firstUse;
}

bool Config::lineWrap() const
{
    return lineWrap_;
}

void Config::setLineWrap(bool lineWrap)
{
    lineWrap_ = lineWrap;
}

bool Config::checkSpelling() const
{
    return checkSpelling_;
}

void Config::setCheckSpelling(bool checkSpelling)
{
    checkSpelling_ = checkSpelling;
}

bool Config::toolbarSearchAdded() const
{
    return toolbarSearchAdded_;
}

void Config::setToolbarSearchAdded(bool toolbarSearchAdded)
{
    toolbarSearchAdded_ = toolbarSearchAdded;
}

bool Config::toolbarZoomAdded() const
{
    return toolbarZoomAdded_;
}

void Config::setToolbarZoomAdded(bool toolbarZoomAdded)
{
    toolbarZoomAdded_ = toolbarZoomAdded;
}

bool Config::toolbarCopyPasteCutAdded() const
{
    return toolbarCopyPasteCutAdded_;
}

void Config::setToolbarCopyPasteCutAdded(bool toolbarCopyPasteCutAdded)
{
    toolbarCopyPasteCutAdded_ = toolbarCopyPasteCutAdded;
}

bool Config::toolbarUndoRedoAdded() const
{
    return toolbarUndoRedoAdded_;
}

void Config::setToolbarUndoRedoAdded(bool toolbarUndoRedoAdded)
{
    toolbarUndoRedoAdded_ = toolbarUndoRedoAdded;
}

bool Config::toolbarFileAdded() const
{
    return toolbarFileAdded_;
}

void Config::setToolbarFileAdded(bool toolbarFileAdded)
{
    toolbarFileAdded_ = toolbarFileAdded;
}

QString Config::style() const
{
    return style_;
}

void Config::setStyle(const QString &style)
{
    style_ = style;
}

int Config::uiSize() const
{
    return uiSize_;
}

void Config::setUiSize(const int &uiSize)
{
    uiSize_ = uiSize;
}

QTabWidget::TabPosition Config::getTabPosition() const
{
    return tabsPosition_;
}

void Config::setTabPosition(QTabWidget::TabPosition position)
{
    tabsPosition_ = position;
}
