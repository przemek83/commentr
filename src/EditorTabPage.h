#ifndef EDITORTABPAGE_H
#define EDITORTABPAGE_H

#include <QWidget>
#include <memory>

namespace Ui
{
class EditorTabPage;
}  // namespace Ui

class CodeViewer;
class Highlighter;
class File;
class Config;
class SpellChecker;

class EditorTabPage : public QWidget
{
    Q_OBJECT
public:
    enum class EditorMode
    {
        PLAIN_TEXT = 0,
        C_CPP,
        JAVA,
        OBJECTIVE_C,
        C_SHARP,
        PHP,
        VISUAL_BASIC,
        PYTHON,
        SQL,
        JAVASCRIPT
    };

    EditorTabPage(File* file, float fontSize, Config& config,
                  SpellChecker& spellChecker, QWidget* parent = nullptr);

    ~EditorTabPage() override;

    void flipFindVisibility();

    void zoom(bool in);

    bool undoAvailable() const;

    bool redoAvailable() const;

    void redo();

    void undo();

    bool selectionEmpty() const;

    void copy();

    void cut();

    void paste();

    void setLineWrap(bool wrap);

    /// @brief change path, base name and suffix of current File object.
    /// @param file file object.
    void changeFile(File* file);

    QString getCurrentText() const;

    EditorMode mode() const;
    void setMode(const EditorMode& mode);

    void refreshVisualIndicators();

    File* getCurrentFileCopy();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    Q_DISABLE_COPY_MOVE(EditorTabPage)

    std::unique_ptr<Ui::EditorTabPage> ui_;

    CodeViewer* codeViewer_;

    bool undoAvailable_{false};

    bool redoAvailable_{false};

    bool selectionEmpty_{true};

    EditorMode mode_{EditorMode::PLAIN_TEXT};

    std::unique_ptr<Highlighter> getHighlighterForEditorMode(EditorMode mode);

    const std::array<QString, 10> modeNames_ = {
        "Plain text", "C/C++",        "Java",   "Objective-C", "C#",
        "PHP",        "Visual Basic", "Python", "SQL",         "JavaScript"};

    static EditorMode detectModeUsingSuffix(const QString& suffix);

    std::unique_ptr<Highlighter> highlighter_{nullptr};

    File* file_;

    Config& config_;

    SpellChecker& spellChecker_;

private slots:
    void searchStringChanged(const QString& string);

    void searchNext();

    void searchPrev();

    void redoAvailabilityChanged(bool available);

    void undoAvailabilityChanged(bool available);

    void copyAndCutAvailable(bool available);

signals:
    void redoIsAvailable(bool available);

    void undoIsAvailable(bool available);

    void copyCutIsAvailable(bool available);
};

#endif  // EDITORTABPAGE_H
