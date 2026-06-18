#include "EditorTabPage.h"

#include <QKeyEvent>
#include <QStyle>

#include "BackButtonHandler.h"
#include "CodeViewer.h"
#include "Config.h"
#include "File.h"
#include "Highlighters/CHighlighter.h"
#include "Highlighters/CSharpHighlighter.h"
#include "Highlighters/CplusPlusHighlighter.h"
#include "Highlighters/Highlighter.h"
#include "Highlighters/JavaHighlighter.h"
#include "Highlighters/JavaScriptHighlighter.h"
#include "Highlighters/ObjectiveCHighlighter.h"
#include "Highlighters/PhpHighlighter.h"
#include "Highlighters/PythonHighlighter.h"
#include "Highlighters/SQLHighlighter.h"
#include "Highlighters/VisualBasicHighlighter.h"
#include "SpellChecker.h"
#include "ui_EditorTabPage.h"

EditorTabPage::EditorTabPage(File file, float fontSize, Config& config,
                             SpellChecker& spellChecker, QWidget* parent)
    : QWidget(parent),
      ui_{std::make_unique<Ui::EditorTabPage>()},
      codeViewer_(new CodeViewer(config, this)),
      file_{std::move(file)},
      config_{config},
      spellChecker_{spellChecker}
{
    ui_->setupUi(this);

    QFont actualEditorFont(codeViewer_->font());
    actualEditorFont.setPointSizeF(fontSize);
    codeViewer_->setFont(actualEditorFont);

    ui_->verticalLayout->insertWidget(0, codeViewer_);

    codeViewer_->setPlainText(file_.content());
    file_.clearContent();

    const QStyle* style{QApplication::style()};
    ui_->next->setIcon(style->standardIcon(QStyle::SP_ArrowRight));
    ui_->prev->setIcon(style->standardIcon(QStyle::SP_ArrowLeft));
    ui_->close->setIcon(style->standardIcon(QStyle::SP_DialogCloseButton));

    ui_->findWidget->hide();

    connect(ui_->close, &QPushButton::clicked, ui_->findWidget, &QWidget::hide);

    connect(ui_->next, &QPushButton::clicked, this, &EditorTabPage::searchNext);

    connect(ui_->prev, &QPushButton::clicked, this, &EditorTabPage::searchPrev);

    connect(ui_->searchLineEdit, &QLineEdit::textChanged, this,
            &EditorTabPage::searchStringChanged);

    connect(ui_->searchLineEdit, &QLineEdit::returnPressed, this,
            &EditorTabPage::searchNext);

    connect(codeViewer_, &CodeViewer::redoAvailable, this,
            &EditorTabPage::redoAvailabilityChanged);

    connect(codeViewer_, &CodeViewer::undoAvailable, this,
            &EditorTabPage::undoAvailabilityChanged);

    connect(codeViewer_, &CodeViewer::copyAvailable, this,
            &EditorTabPage::copyAndCutAvailable);

    setMode(detectModeUsingSuffix(file_.suffix()));

    ui_->searchLineEdit->setInputMethodHints(Qt::ImhNoPredictiveText);

    auto* backButtonHandler{new BackButtonHandler(this)};
    codeViewer_->installEventFilter(backButtonHandler);
    ui_->searchLineEdit->installEventFilter(backButtonHandler);
}

EditorTabPage::~EditorTabPage() = default;

void EditorTabPage::keyPressEvent(QKeyEvent* event)
{
    if (event->matches(QKeySequence::Find))
        flipFindVisibility();

    QWidget::keyPressEvent(event);
}

SyntaxLang EditorTabPage::mode() const { return mode_; }

void EditorTabPage::setMode(SyntaxLang mode)
{
    if (mode == mode_)
        return;

    mode_ = mode;

    highlighter_ = getHighlighterForEditorMode(mode);

    if (highlighter_ != nullptr)
        highlighter_->setDocument(codeViewer_->document());
}

void EditorTabPage::flipFindVisibility()
{
    ui_->findWidget->setVisible(!ui_->findWidget->isVisible());

    if (ui_->findWidget->isVisible())
        ui_->searchLineEdit->setFocus();
}

// 16.4.2018 Needed?
void EditorTabPage::zoom(bool in)
{
    if (in)
        codeViewer_->zoom(1);
    else
        codeViewer_->zoom(-1);

    const float newSize{
        static_cast<float>(codeViewer_->fontInfo().pointSizeF())};
    config_.setFontSize(newSize);
}

void EditorTabPage::searchNext()
{
    QTextDocument::FindFlags flags;
    if (!ui_->ignoreCase->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    const bool found{codeViewer_->find(ui_->searchLineEdit->text(), flags)};

    if (!found)
    {
        codeViewer_->moveCursor(QTextCursor::Start);
        codeViewer_->find(ui_->searchLineEdit->text(), flags);
    }
}

void EditorTabPage::searchPrev()
{
    QTextDocument::FindFlags flags = QTextDocument::FindBackward;
    if (!ui_->ignoreCase->isChecked())
    {
        flags |= QTextDocument::FindCaseSensitively;
    }

    const bool found{codeViewer_->find(ui_->searchLineEdit->text(), flags)};
    if (!found)
    {
        codeViewer_->moveCursor(QTextCursor::End);
        codeViewer_->find(ui_->searchLineEdit->text(), flags);
    }
}

void EditorTabPage::searchStringChanged([[maybe_unused]] const QString& string)
{
    QTextCursor cursor = codeViewer_->textCursor();
    cursor.setPosition(cursor.anchor());
    codeViewer_->setTextCursor(cursor);
    searchNext();
}

bool EditorTabPage::undoAvailable() const { return undoAvailable_; }

bool EditorTabPage::redoAvailable() const { return redoAvailable_; }

bool EditorTabPage::selectionEmpty() const { return selectionEmpty_; }

void EditorTabPage::redoAvailabilityChanged(bool available)
{
    redoAvailable_ = available;

    emit redoIsAvailable(redoAvailable_);
}

void EditorTabPage::undoAvailabilityChanged(bool available)
{
    undoAvailable_ = available;

    emit undoIsAvailable(undoAvailable_);
}

void EditorTabPage::redo() { codeViewer_->redo(); }

void EditorTabPage::undo() { codeViewer_->undo(); }

void EditorTabPage::copyAndCutAvailable(bool available)
{
    selectionEmpty_ = !available;

    emit copyCutIsAvailable(available);
}

void EditorTabPage::copy() { codeViewer_->copy(); }

void EditorTabPage::cut() { codeViewer_->cut(); }

void EditorTabPage::paste() { codeViewer_->paste(); }

void EditorTabPage::setLineWrap(bool wrap)
{
    if (wrap)
        codeViewer_->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    else
        codeViewer_->setLineWrapMode(QPlainTextEdit::NoWrap);
}

void EditorTabPage::changeFile(const File& file)
{
    file_.setFilePath(file.getFilePath());
}

QString EditorTabPage::getCurrentText() const
{
    return codeViewer_->toPlainText();
}

std::unique_ptr<Highlighter> EditorTabPage::getHighlighterForEditorMode(
    SyntaxLang mode)
{
    switch (mode)
    {
        case SyntaxLang::CPP:
            return std::make_unique<CplusPlusHighlighter>(spellChecker_);

        case SyntaxLang::C:
            return std::make_unique<CHighlighter>(spellChecker_);

        case SyntaxLang::JAVA:
            return std::make_unique<JavaHighlighter>(spellChecker_);

        case SyntaxLang::OBJECTIVE_C:
            return std::make_unique<ObjectiveCHighlighter>(spellChecker_);

        case SyntaxLang::C_SHARP:
            return std::make_unique<CSharpHighlighter>(spellChecker_);

        case SyntaxLang::PHP:
            return std::make_unique<PhpHighlighter>(spellChecker_);

        case SyntaxLang::VISUAL_BASIC:
            return std::make_unique<VisualBasicHighlighter>(spellChecker_);

        case SyntaxLang::PYTHON:
            return std::make_unique<PythonHighlighter>(spellChecker_);

        case SyntaxLang::SQL:
            return std::make_unique<SQLHighlighter>(spellChecker_);

        case SyntaxLang::JAVASCRIPT:
            return std::make_unique<JavaScriptHighlighter>(spellChecker_);

        default:
            return nullptr;
    }
}

void EditorTabPage::refreshVisualIndicators()
{
    codeViewer_->updateVisualPointersPositions();
}

File EditorTabPage::getCurrentFileCopy() const
{
    File file(file_.source(), file_.getFilePath(), codeViewer_->toPlainText());

    return file;
}

void EditorTabPage::refreshHighlighter()
{
    if (highlighter_ != nullptr)
        highlighter_->rehighlight();
}

SyntaxLang EditorTabPage::detectModeUsingSuffix(const QString& suffix)
{
    SyntaxLang mode{SyntaxLang::PLAIN_TEXT};
    if (suffix == QLatin1String("java"))
        mode = SyntaxLang::JAVA;

    const bool isCPlusPlusSourceFile{
        (suffix == QLatin1String("cpp")) || (suffix == QLatin1String("cxx")) ||
        (suffix == QLatin1String("c++")) || (suffix == QLatin1String("cc"))};
    const bool isCPlusPlusHeaderFile{(suffix == QLatin1String("h")) ||
                                     (suffix == QLatin1String("hpp"))};
    const bool isMocFile{suffix == QLatin1String("moc")};

    if (isCPlusPlusSourceFile || isCPlusPlusHeaderFile || isMocFile)
        mode = SyntaxLang::CPP;

    const bool isCSourceFile{suffix == QLatin1String("c")};
    if (isCSourceFile)
        mode = SyntaxLang::C;

    if (suffix == QLatin1String("m"))
        mode = SyntaxLang::OBJECTIVE_C;

    if (suffix == QLatin1String("cs"))
        mode = SyntaxLang::C_SHARP;

    if (suffix == QLatin1String("php"))
        mode = SyntaxLang::PHP;

    if ((suffix == QLatin1String("bas")) || (suffix == QLatin1String("cls")) ||
        (suffix == QLatin1String("vb")))
        mode = SyntaxLang::VISUAL_BASIC;

    if (suffix == QLatin1String("py"))
        mode = SyntaxLang::PYTHON;

    if (suffix == QLatin1String("sql"))
        mode = SyntaxLang::SQL;

    if (suffix == QLatin1String("js"))
        mode = SyntaxLang::JAVASCRIPT;

    return mode;
}
