#include <QDebug>
#include <QKeyEvent>
#include <QStyle>

#include "EditorTabPage.h"
#include "ui_EditorTabPage.h"
#include "CodeViewer.h"
#include "Highlighters/CplusPlusHighlighter.h"
#include "Highlighters/JavaHighlighter.h"
#include "Highlighters/Highlighter.h"
#include "Highlighters/ObjectiveCHighlighter.h"
#include "Highlighters/CSharpHighlighter.h"
#include "Highlighters/PhpHighlighter.h"
#include "Highlighters/BasicHighlighter.h"
#include "Highlighters/PythonHighlighter.h"
#include "Highlighters/SQLHighlighter.h"
#include "Highlighters/JavaScriptHighlighter.h"
#include "BackButtonHandler.h"
#include "File.h"
#include "Config.h"

const char* EditorTabPage::modeNames_[] =
{
    "Plain text",
    "C/C++",
    "Java",
    "Objective-C",
    "C#",
    "PHP",
    "Visual Basic",
    "Python",
    "SQL",
    "JavaScript"
};

EditorTabPage::EditorTabPage(File* file, float fontSize, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::EditorTabPage),
    undoAvailable_(false),
    redoAvailable_(false),
    selectionEmpty_(true),
    file_(file)
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

    connect(ui->close, SIGNAL(clicked()), ui->findWidget, SLOT(hide()));

    connect(ui->next, SIGNAL(clicked()), this, SLOT(searchNext()));

    connect(ui->prev, SIGNAL(clicked()), this, SLOT(searchPrev()));

    connect(ui->searchLineEdit,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(searchStringChanged(QString)));

    connect(ui->searchLineEdit,
            SIGNAL(returnPressed()),
            this,
            SLOT(searchNext()));

    connect(codeViewer_,
            SIGNAL(redoAvailable(bool)),
            this,
            SLOT(redoAvailabilityChanged(bool)));

    connect(codeViewer_,
            SIGNAL(undoAvailable(bool)),
            this,
            SLOT(undoAvailabilityChanged(bool)));

    connect(codeViewer_,
            SIGNAL(copyAvailable(bool)),
            this,
            SLOT(copyAndCutAvailable(bool)));

    //Init highlighter.
    setMode(detectModeUsingSuffix(file->suffix()));

    ui->searchLineEdit->setInputMethodHints(Qt::ImhNoPredictiveText);

    BackButtonHandler* backButtonHandler = new BackButtonHandler(this);
    codeViewer_->installEventFilter(backButtonHandler);
    ui->searchLineEdit->installEventFilter(backButtonHandler);
}

EditorTabPage::~EditorTabPage()
{
    delete ui;

    if( NULL != file_ )
    {
        delete file_;
    }
}

void EditorTabPage::keyPressEvent(QKeyEvent* event)
{
    if( true == event->matches(QKeySequence::Find) )
    {
       flipFindVisibility();
    }
    QWidget::keyPressEvent(event);
}

EditorTabPage::EditorMode EditorTabPage::mode() const
{
    return mode_;
}

void EditorTabPage::setMode(const EditorMode &mode)
{
    if( mode == mode_)
    {
        return;
    }

    mode_ = mode;

    QList<QSyntaxHighlighter*> highlighters = findChildren<QSyntaxHighlighter*>();
    foreach( QSyntaxHighlighter* highlighter, highlighters )
    {
        delete highlighter;
    }

    Highlighter* highlighter = getHighlighterForEditorMode(mode);

    if( NULL != highlighter )
    {
        highlighter->setDocument(codeViewer_->document());
    }
}

void EditorTabPage::flipFindVisibility()
{
    ui->findWidget->setVisible(!ui->findWidget->isVisible());

    if( true == ui->findWidget->isVisible() )
    {
        ui->searchLineEdit->setFocus();
    }
}

//16.4.2018 Needed?
void EditorTabPage::zoom(bool in)
{
    codeViewer_->zoom(in ? 1 : -1);

    Config::getInstance().setFontSize(codeViewer_->fontInfo().pointSizeF());
}

void EditorTabPage::searchNext()
{
    QTextDocument::FindFlags flags;
    if( false == ui->ignoreCase->isChecked() )
    {
        flags |= QTextDocument::FindCaseSensitively;
    }

    bool found = codeViewer_->find(ui->searchLineEdit->text(), flags);

    if( false == found )
    {
        codeViewer_->moveCursor(QTextCursor::Start);
        codeViewer_->find(ui->searchLineEdit->text(), flags);
    }
    /*else
    {
        codeViewer_->scrollToSelection();
    }*/
}

void EditorTabPage::searchPrev()
{
    QTextDocument::FindFlags flags = QTextDocument::FindBackward;
    if( false == ui->ignoreCase->isChecked() )
    {
        flags |= QTextDocument::FindCaseSensitively;
    }

    bool found = codeViewer_->find(ui->searchLineEdit->text(), flags);
    if( false == found )
    {
        codeViewer_->moveCursor(QTextCursor::End);
        codeViewer_->find(ui->searchLineEdit->text(), flags);
    }
    /*else
    {
        codeViewer_->scrollToSelection();
    }*/
}

void EditorTabPage::searchStringChanged(QString /*string*/)
{
//    qDebug() << "Window" << parentWidget()->geometry() << mapToGlobal(QPoint(0,0));

    QTextCursor cursor = codeViewer_->textCursor();
    cursor.setPosition(cursor.anchor());
    codeViewer_->setTextCursor(cursor);
    searchNext();
    //codeViewer_->scrollToSelection();
}

bool EditorTabPage::undoAvailable()
{
    return undoAvailable_;
}

bool EditorTabPage::redoAvailable()
{
    return redoAvailable_;
}

bool EditorTabPage::selectionEmpty()
{
    return selectionEmpty_;
}

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

void EditorTabPage::redo()
{
    codeViewer_->redo();
}

void EditorTabPage::undo()
{
    codeViewer_->undo();
}

void EditorTabPage::copyAndCutAvailable(bool available)
{
    selectionEmpty_ = !available;

    emit copyCutAvailable(available);
}

void EditorTabPage::copy()
{
    codeViewer_->copy();
}

void EditorTabPage::cut()
{
    codeViewer_->cut();
}

void EditorTabPage::paste()
{
    codeViewer_->paste();
}

void EditorTabPage::setLineWrap(bool wrap)
{
    if( true == wrap )
    {
        codeViewer_->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    }
    else
    {
        codeViewer_->setLineWrapMode(QPlainTextEdit::NoWrap);
    }
}

void EditorTabPage::changeFile(File* file)
{
    file_->setSource(file->source());
    file_->setPath(file->path());
    file_->setBaseName(file->baseName());
    file_->setSuffix(file->suffix());
}

QString EditorTabPage::getCurrentText()
{
    return codeViewer_->toPlainText();
}

QString EditorTabPage::getModeName()
{
    return modeNames_[mode_];
}

Highlighter* EditorTabPage::getHighlighterForEditorMode(EditorMode mode)
{
    switch(mode)
    {
        case EDITOR_MODE_C_CPP:
        {
            return new CplusPlusHighlighter(this);
        }

        case EDITOR_MODE_JAVA:
        {
            return new JavaHighlighter(this);
        }

        case EDITOR_MODE_OBJECTIVE_C:
        {
            return new ObjectiveCHighlighter(this);
        }

        case EDITOR_MODE_C_SHARP:
        {
            return new CSharpHighlighter(this);
        }

        case EDITOR_MODE_PHP:
        {
            return new PhpHighlighter(this);
        }

        case EDITOR_MODE_VISUAL_BASIC:
        {
            return new BasicHighlighter(this);
        }

        case EDITOR_MODE_PYTHON:
        {
            return new PythonHighlighter(this);
        }

        case EDITOR_MODE_SQL:
        {
            return new SQLHighlighter(this);
        }

        case EDITOR_MODE_JAVASCRIPT:
        {
            return new JavaScriptHighlighter(this);
        }

        default:
        {
            return NULL;
        }
    }
}

void EditorTabPage::refreshVisualIndicators()
{
    codeViewer_->updateVisualPointersPositions();
}

File* EditorTabPage::getCurrentFileCopy()
{
    //TODO: Performance issue due to copy of content?
    File* file = new File(file_->source(),
                          file_->path(),
                          file_->baseName(),
                          file_->suffix(),
                          new QString(codeViewer_->toPlainText()));

    return file;
}

EditorTabPage::EditorMode EditorTabPage::detectModeUsingSuffix(QString suffix)
{
    if( suffix == "java" )
    {
        return EditorTabPage::EDITOR_MODE_JAVA;
    }

    if( suffix == "c" || suffix == "cpp" || suffix == "h" || suffix == "hpp" ||
        suffix == "cc" || suffix == "cxx" || suffix == "c++" || suffix == "moc" )
    {
        return EditorTabPage::EDITOR_MODE_C_CPP;
    }

    if( suffix == "m" )
    {
        return EditorTabPage::EDITOR_MODE_OBJECTIVE_C;
    }

    if( suffix == "cs" )
    {
        return EditorTabPage::EDITOR_MODE_C_SHARP;
    }

    if( suffix == "php" )
    {
        return EditorTabPage::EDITOR_MODE_PHP;
    }

    if( suffix == "bas" || suffix == "cls" || suffix == "vb" )
    {
        return EditorTabPage::EDITOR_MODE_VISUAL_BASIC;
    }

    if( suffix == "py" )
    {
        return EditorTabPage::EDITOR_MODE_PYTHON;
    }

    if( suffix == "sql" )
    {
        return EditorTabPage::EDITOR_MODE_SQL;
    }

    if( suffix == "js" )
    {
        return EditorTabPage::EDITOR_MODE_JAVASCRIPT;
    }

    return EditorTabPage::EDITOR_MODE_PLAIN_TEXT;
}

