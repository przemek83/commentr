#include "EditorTabPage.h"

#include <QKeyEvent>
#include <QStyle>

#include "BackButtonHandler.h"
#include "CodeViewer.h"
#include "Config.h"
#include "File.h"
#include "Highlighters/BasicHighlighter.h"
#include "Highlighters/CSharpHighlighter.h"
#include "Highlighters/CplusPlusHighlighter.h"
#include "Highlighters/Highlighter.h"
#include "Highlighters/JavaHighlighter.h"
#include "Highlighters/JavaScriptHighlighter.h"
#include "Highlighters/ObjectiveCHighlighter.h"
#include "Highlighters/PhpHighlighter.h"
#include "Highlighters/PythonHighlighter.h"
#include "Highlighters/SQLHighlighter.h"
#include "ui_EditorTabPage.h"

EditorTabPage::EditorTabPage(File* file, float fontSize, QWidget* parent)
    : QWidget(parent), ui{new Ui::EditorTabPage}, file_{file}
{
    ui->setupUi(this);

    codeViewer_ = new CodeViewer(this);
    QFont actualEditorFont(codeViewer_->font());
    actualEditorFont.setPointSizeF(fontSize);
    codeViewer_->setFont(actualEditorFont);

    ui->verticalLayout->insertWidget(0, codeViewer_);

    codeViewer_->setPlainText(*(file->content()));
    file->clearContent();

    QStyle* style = QApplication::style();
    ui->next->setIcon(style->standardIcon(QStyle::SP_ArrowRight));
    ui->prev->setIcon(style->standardIcon(QStyle::SP_ArrowLeft));
    ui->close->setIcon(style->standardIcon(QStyle::SP_DialogCloseButton));

    ui->findWidget->hide();

    connect(ui->close, &QPushButton::clicked, ui->findWidget, &QWidget::hide);

    connect(ui->next, &QPushButton::clicked, this, &EditorTabPage::searchNext);

    connect(ui->prev, &QPushButton::clicked, this, &EditorTabPage::searchPrev);

    connect(ui->searchLineEdit, &QLineEdit::textChanged, this,
            &EditorTabPage::searchStringChanged);

    connect(ui->searchLineEdit, &QLineEdit::returnPressed, this,
            &EditorTabPage::searchNext);

    connect(codeViewer_, &CodeViewer::redoAvailable, this,
            &EditorTabPage::redoAvailabilityChanged);

    connect(codeViewer_, &CodeViewer::undoAvailable, this,
            &EditorTabPage::undoAvailabilityChanged);

    connect(codeViewer_, &CodeViewer::copyAvailable, this,
            &EditorTabPage::copyAndCutAvailable);

    // Init highlighter.
    setMode(detectModeUsingSuffix(file->suffix()));

    ui->searchLineEdit->setInputMethodHints(Qt::ImhNoPredictiveText);

    auto* backButtonHandler{new BackButtonHandler(this)};
    codeViewer_->installEventFilter(backButtonHandler);
    ui->searchLineEdit->installEventFilter(backButtonHandler);
}

EditorTabPage::~EditorTabPage()
{
    delete ui;
    delete file_;
}

void EditorTabPage::keyPressEvent(QKeyEvent* event)
{
    if (event->matches(QKeySequence::Find))
        flipFindVisibility();

    QWidget::keyPressEvent(event);
}

EditorTabPage::EditorMode EditorTabPage::mode() const { return mode_; }

void EditorTabPage::setMode(const EditorMode& mode)
{
    if (mode == mode_)
        return;

    mode_ = mode;

    QList<QSyntaxHighlighter*> highlighters{
        findChildren<QSyntaxHighlighter*>()};
    foreach (QSyntaxHighlighter* highlighter, highlighters)
        delete highlighter;

    Highlighter* highlighter{getHighlighterForEditorMode(mode)};

    if (highlighter != nullptr)
        highlighter->setDocument(codeViewer_->document());
}

void EditorTabPage::flipFindVisibility()
{
    ui->findWidget->setVisible(!ui->findWidget->isVisible());

    if (ui->findWidget->isVisible())
        ui->searchLineEdit->setFocus();
}

// 16.4.2018 Needed?
void EditorTabPage::zoom(bool in)
{
    if (in)
        codeViewer_->zoom(1.F);
    else
        codeViewer_->zoom(-1.F);

    const float newSize{
        static_cast<float>(codeViewer_->fontInfo().pointSizeF())};
    Config::getInstance().setFontSize(newSize);
}

void EditorTabPage::searchNext()
{
    QTextDocument::FindFlags flags;
    if (!ui->ignoreCase->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    const bool found{codeViewer_->find(ui->searchLineEdit->text(), flags)};

    if (!found)
    {
        codeViewer_->moveCursor(QTextCursor::Start);
        codeViewer_->find(ui->searchLineEdit->text(), flags);
    }
}

void EditorTabPage::searchPrev()
{
    QTextDocument::FindFlags flags = QTextDocument::FindBackward;
    if (!ui->ignoreCase->isChecked())
    {
        flags |= QTextDocument::FindCaseSensitively;
    }

    const bool found{codeViewer_->find(ui->searchLineEdit->text(), flags)};
    if (!found)
    {
        codeViewer_->moveCursor(QTextCursor::End);
        codeViewer_->find(ui->searchLineEdit->text(), flags);
    }
}

void EditorTabPage::searchStringChanged([[maybe_unused]] QString string)
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

    emit redoAvailable(redoAvailable_);
}

void EditorTabPage::undoAvailabilityChanged(bool available)
{
    undoAvailable_ = available;

    emit undoAvailable(undoAvailable_);
}

void EditorTabPage::redo() { codeViewer_->redo(); }

void EditorTabPage::undo() { codeViewer_->undo(); }

void EditorTabPage::copyAndCutAvailable(bool available)
{
    selectionEmpty_ = !available;

    emit copyCutAvailable(available);
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

void EditorTabPage::changeFile(File* file)
{
    file_->setSource(file->source());
    file_->setPath(file->path());
    file_->setBaseName(file->baseName());
    file_->setSuffix(file->suffix());
}

QString EditorTabPage::getCurrentText() const
{
    return codeViewer_->toPlainText();
}

QString EditorTabPage::getModeName() const { return modeNames_[mode_]; }

Highlighter* EditorTabPage::getHighlighterForEditorMode(EditorMode mode)
{
    switch (mode)
    {
        case EDITOR_MODE_C_CPP:
            return new CplusPlusHighlighter(this);

        case EDITOR_MODE_JAVA:
            return new JavaHighlighter(this);

        case EDITOR_MODE_OBJECTIVE_C:
            return new ObjectiveCHighlighter(this);

        case EDITOR_MODE_C_SHARP:
            return new CSharpHighlighter(this);

        case EDITOR_MODE_PHP:
            return new PhpHighlighter(this);

        case EDITOR_MODE_VISUAL_BASIC:
            return new BasicHighlighter(this);

        case EDITOR_MODE_PYTHON:
            return new PythonHighlighter(this);

        case EDITOR_MODE_SQL:
            return new SQLHighlighter(this);

        case EDITOR_MODE_JAVASCRIPT:
            return new JavaScriptHighlighter(this);

        default:
            return nullptr;
    }
}

void EditorTabPage::refreshVisualIndicators()
{
    codeViewer_->updateVisualPointersPositions();
}

File* EditorTabPage::getCurrentFileCopy()
{
    auto* file{new File(file_->source(), file_->path(), file_->baseName(),
                        file_->suffix(),
                        new QString(codeViewer_->toPlainText()))};

    return file;
}

EditorTabPage::EditorMode EditorTabPage::detectModeUsingSuffix(
    const QString& suffix)
{
    EditorTabPage::EditorMode mode{EDITOR_MODE_PLAIN_TEXT};
    if (suffix == "java")
        mode = EditorTabPage::EDITOR_MODE_JAVA;

    if (suffix == "c" || suffix == "cpp" || suffix == "h" || suffix == "hpp" ||
        suffix == "cc" || suffix == "cxx" || suffix == "c++" || suffix == "moc")
        mode = EditorTabPage::EDITOR_MODE_C_CPP;

    if (suffix == "m")
        mode = EditorTabPage::EDITOR_MODE_OBJECTIVE_C;

    if (suffix == "cs")
        mode = EditorTabPage::EDITOR_MODE_C_SHARP;

    if (suffix == "php")
        mode = EditorTabPage::EDITOR_MODE_PHP;

    if (suffix == "bas" || suffix == "cls" || suffix == "vb")
        mode = EditorTabPage::EDITOR_MODE_VISUAL_BASIC;

    if (suffix == "py")
        mode = EditorTabPage::EDITOR_MODE_PYTHON;

    if (suffix == "sql")
        mode = EditorTabPage::EDITOR_MODE_SQL;

    if (suffix == "js")
        mode = EditorTabPage::EDITOR_MODE_JAVASCRIPT;

    return mode;
}
