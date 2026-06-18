#ifndef EDITORTABPAGE_H
#define EDITORTABPAGE_H

#include <memory>

#include <QWidget>

#include "File.h"
#include "SyntaxLang.h"

namespace Ui
{
class EditorTabPage;
}  // namespace Ui

class CodeViewer;
class Highlighter;
class Config;
class SpellChecker;

class EditorTabPage : public QWidget
{
    Q_OBJECT
public:
    EditorTabPage(File file, float fontSize, Config& config,
                  SpellChecker& spellChecker, QWidget* parent);

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
    void changeFile(const File& file);

    QString getCurrentText() const;

    SyntaxLang mode() const;
    void setMode(SyntaxLang mode);

    void refreshVisualIndicators();

    File getCurrentFileCopy() const;

    void refreshHighlighter();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    Q_DISABLE_COPY_MOVE(EditorTabPage)

    std::unique_ptr<Ui::EditorTabPage> ui_;

    CodeViewer* codeViewer_;

    bool undoAvailable_{false};

    bool redoAvailable_{false};

    bool selectionEmpty_{true};

    SyntaxLang mode_{SyntaxLang::PLAIN_TEXT};

    std::unique_ptr<Highlighter> getHighlighterForEditorMode(SyntaxLang mode);

    const std::array<QString, 10> modeNames_ = {
        "Plain text", "C/C++",        "Java",   "Objective-C", "C#",
        "PHP",        "Visual Basic", "Python", "SQL",         "JavaScript"};

    static SyntaxLang detectModeUsingSuffix(const QString& suffix);

    std::unique_ptr<Highlighter> highlighter_{nullptr};

    File file_;

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
