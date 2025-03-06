#ifndef EDITORTABPAGE_H
#define EDITORTABPAGE_H

#include <QWidget>

namespace Ui {
class EditorTabPage;
}

class CodeViewer;
class Highlighter;
class File;

class EditorTabPage : public QWidget
{
    Q_OBJECT
public:
    enum EditorMode
    {
        EDITOR_MODE_PLAIN_TEXT = 0,
        EDITOR_MODE_C_CPP,
        EDITOR_MODE_JAVA,
        EDITOR_MODE_OBJECTIVE_C,
        EDITOR_MODE_C_SHARP,
        EDITOR_MODE_PHP,
        EDITOR_MODE_VISUAL_BASIC,
        EDITOR_MODE_PYTHON,
        EDITOR_MODE_SQL,
        EDITOR_MODE_JAVASCRIPT
    };

    EditorTabPage(File* file, float fontSize, QWidget* parent = 0);

    virtual ~EditorTabPage();

    void flipFindVisibility();

    void zoom(bool in);

    bool undoAvailable();

    bool redoAvailable();

    void redo();

    void undo();

    bool selectionEmpty();

    void copy();

    void cut();

    void paste();

    void setLineWrap(bool wrap);

    /**
     * @brief change path, base name and suffix of current File object.
     * @param file file object.
     */
    void changeFile(File* file);

    QString getCurrentText();

    EditorMode mode() const;
    void setMode(const EditorMode& mode);

    QString getModeName();

    void refreshVisualIndicators();

    File* getCurrentFileCopy();

protected:
    virtual void keyPressEvent(QKeyEvent* event);

private:
    Q_DISABLE_COPY(EditorTabPage)

    Ui::EditorTabPage *ui;

    CodeViewer* codeViewer_;

    bool undoAvailable_;

    bool redoAvailable_;

    bool selectionEmpty_;

    EditorMode mode_;

    Highlighter* getHighlighterForEditorMode(EditorMode mode);

    const static char* modeNames_[];

    EditorMode detectModeUsingSuffix(QString suffix);

    File* file_;

private slots:
    void searchStringChanged(QString string);

    void searchNext();

    void searchPrev();

    void redoAvailabilityChanged(bool available);

    void undoAvailabilityChanged(bool available);

    void copyAndCutAvailable(bool available);

signals:
    void redoAvailable(bool);

    void undoAvailable(bool);

    void copyCutAvailable(bool);
};

#endif // EDITORTABPAGE_H
