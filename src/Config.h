#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QTabWidget>

class QSettings;

class Config : public QObject
{
    Q_OBJECT

public:
    static Config& getInstance();

    QTabWidget::TabPosition getTabPosition() const;
    void setTabPosition(QTabWidget::TabPosition position);

    float uiSize() const;
    void setUiSize(const float& uiSize);

    QString style() const;
    void setStyle(const QString& style);

    bool toolbarFileAdded() const;
    void setToolbarFileAdded(bool toolbarFileAdded);

    bool toolbarUndoRedoAdded() const;
    void setToolbarUndoRedoAdded(bool toolbarUndoRedoAdded);

    bool toolbarCopyPasteCutAdded() const;
    void setToolbarCopyPasteCutAdded(bool toolbarCopyPasteCutAdded);

    bool toolbarZoomAdded() const;
    void setToolbarZoomAdded(bool toolbarZoomAdded);

    bool toolbarSearchAdded() const;
    void setToolbarSearchAdded(bool toolbarSearchAdded);

    bool checkSpelling() const;
    void setCheckSpelling(bool checkSpelling);

    bool lineWrap() const;
    void setLineWrap(bool lineWrap);

    bool firstUse() const;
    void setFirstUse(bool firstUse);

    void setDefaultFont();

    bool showToolbar() const;
    void setShowToolbar(bool showToolbar);

    Qt::ToolBarArea toolbarArea() const;
    void setToolbarArea(const Qt::ToolBarArea& toolbarArea);

    float fontSize() const;
    void setFontSize(float fontSize);

    bool listViewInBrowser() const;
    void setListViewInBrowser(bool listViewInBrowser);

    bool toolbarKeyboardAdded() const;
    void setToolbarKeyboardAdded(bool toolbarKeyboardAdded);

    QString lastPickedDir() const;
    void setLastPickedDir(const QString& lastPickedDir);

    const QStringList& getRecentFiles() const;
    void addFilePathToRecentFiles(const QString& filePath);

public slots:
    void save();

private:
    Config();
    Q_DISABLE_COPY(Config)

    void load();

    /// Names used.
    const std::array<QString, 18> configNames_{
        "tabsPosition",      "uiSize",           "style",
        "toolbar/file",      "toolbar/undoRedo", "toolbar/copyPasteCut",
        "toolbar/zoom",      "toolbar/search",   "toolbar/keyboard",
        "checkSpelling",     "lineWrap",         "firstUse",
        "showtoolbar",       "toolbarposition",  "fontsize",
        "listViewInBrowser", "lastdir",          "recentFileList"};

    /// Enum used for names.
    enum ConfigNames
    {
        CONFIG_TABS_POSITION = 0,
        CONFIG_UI_SIZE,
        CONFIG_STYLE,
        CONFIG_TOOLBAR_FILE,
        CONFIG_TOOLBAR_UNDO_REDO,
        CONFIG_TOOLBAR_COPY_PASTE_CUT,
        CONFIG_TOOLBAR_ZOOM,
        CONFIG_TOOLBAR_SEARCH,
        CONFIG_TOOLBAR_KEYBOARD,
        CONFIG_CHECK_SPELLING,
        CONFIG_LINE_WRAP,
        CONFIG_FIRST_USE,
        CONFIG_SHOW_TOOLBAR,
        CONFIG_TOOLBAR_POSITION,
        CONFIG_FONT_SIZE,
        CONFIG_LIST_VIEW_IN_BROWSER,
        CONFIG_LAST_DIR,
        CONFIG_RECENT_FILES
    };

    void setValue(QSettings& settings, ConfigNames name,
                  const QVariant& value) const;

    QTabWidget::TabPosition tabsPosition_;

    QString style_;

    float uiSize_;

    bool toolbarFileAdded_;

    bool toolbarUndoRedoAdded_;

    bool toolbarCopyPasteCutAdded_;

    bool toolbarZoomAdded_;

    bool toolbarSearchAdded_;

    bool toolbarKeyboardAdded_;

    bool checkSpelling_;

    bool lineWrap_;

    bool firstUse_;

    bool showToolbar_;

    Qt::ToolBarArea toolbarArea_;

    float fontSize_;

    // If true one column list view in file browser, if false multi column view.
    bool listViewInBrowser_;

    bool keyboardAfterTap_;

    QString lastPickedDir_;

    QStringList recentFiles_;
};

#endif  // CONFIG_H
