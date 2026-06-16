#include "BrowseFilesWidget.h"

#include <QFileInfo>
#include <QMessageBox>

#include "../Common.h"
#include "../Config.h"
#include "../EnhancedLineEdit.h"
#include "FileExplorer.h"
#include "ui_BrowseFilesWidget.h"

BrowseFilesWidget::BrowseFilesWidget(bool open, Config& config, QWidget* parent)
    : QWidget(parent),
      ui_{std::make_unique<Ui::BrowseFilesWidget>()},
      config_(config)
{
    ui_->setupUi(this);

    initLineEdit();

    auto* localListView{new FileExplorer(open, config_, ui_->tabWidget)};

    connect(localListView, &FileExplorer::filePrepared, this,
            &BrowseFilesWidget::filePrepared);

    connect(localListView, &FileExplorer::pathChanged, filePathLineEdit_,
            &EnhancedLineEdit::setText);

    connect(ui_->changeView, &QPushButton::clicked, this,
            &BrowseFilesWidget::changeView);

    connect(ui_->tabWidget, &QTabWidget::currentChanged, this,
            &BrowseFilesWidget::currentTabChanged);

    ui_->tabWidget->insertTab(static_cast<int>(Common::Source::LOCAL),
                              localListView, QStringLiteral("local"));

    setProperIconForViewButton();

    // Set line edit text, do not allow emitting signals
    //(virtual methods can not be called).
    filePathLineEdit_->blockSignals(true);
    filePathLineEdit_->setText(localListView->getCurrentPath());
    filePathLineEdit_->blockSignals(false);

    setAttribute(Qt::WA_AcceptTouchEvents);
}

BrowseFilesWidget::~BrowseFilesWidget() = default;

void BrowseFilesWidget::initLineEdit()
{
    filePathLineEdit_ = new EnhancedLineEdit(config_, this);

    ui_->upperHorizontalLayout->insertWidget(1, filePathLineEdit_);
    filePathLineEdit_->setStyleSheet(
        QStringLiteral("QLineEdit{background: #FFBFBF;}"));

    connect(filePathLineEdit_, &EnhancedLineEdit::returnPressed, this,
            &BrowseFilesWidget::filePathReturnPressed);

    connect(filePathLineEdit_, &EnhancedLineEdit::textChanged, this,
            &BrowseFilesWidget::filePathTextChanged);

    filePathLineEdit_->setInputMethodHints(Qt::ImhNoPredictiveText);
}

void BrowseFilesWidget::filePathReturnPressed()
{
    const int currentIndex{ui_->tabWidget->currentIndex()};
    if (const auto source{static_cast<Common::Source>(currentIndex)};
        source != Common::Source::LOCAL)
        Q_ASSERT(false);

    QString filePath(filePathLineEdit_->text());
    QFileInfo fileInfo(filePath);

    QString canonicalFilePath(fileInfo.canonicalFilePath());
    if (fileInfo.isDir())
    {
        currentListView()->setPath(canonicalFilePath);
        return;
    }

    currentListView()->performOperationOnFile(filePath);
}

void BrowseFilesWidget::filePathTextChanged(const QString& arg1)
{
    if (currentListView()->fileIsValid(arg1))
        filePathLineEdit_->setStyleSheet(
            QStringLiteral("QLineEdit{background: white;}"));
    else
        filePathLineEdit_->setStyleSheet(
            QStringLiteral("QLineEdit{background: #FFBFBF;}"));

    if (filePathLineEdit_->text().isEmpty())
    {
        currentListView()->setPath(QLatin1String(""));
        filePathLineEdit_->setText(Common::rootPath());
    }
}

void BrowseFilesWidget::setProperIconForViewButton()
{
    if (!currentListView()->isWrappingContent())
    {
        QIcon viewIcon =
            QApplication::style()->standardIcon(QStyle::SP_FileDialogListView);
        ui_->changeView->setIcon(viewIcon);
    }
    else
    {
        QIcon viewIcon = QApplication::style()->standardIcon(
            QStyle::SP_FileDialogDetailedView);
        ui_->changeView->setIcon(viewIcon);
    }
}

FileExplorer* BrowseFilesWidget::currentListView()
{
    return dynamic_cast<FileExplorer*>(ui_->tabWidget->currentWidget());
}

void BrowseFilesWidget::changeView()
{
    bool wrapping{currentListView()->isWrappingContent()};
    int tabCount{ui_->tabWidget->count()};
    for (int i{0}; i < tabCount; ++i)
    {
        auto* listView{dynamic_cast<QListView*>(ui_->tabWidget->widget(i))};
        listView->setWrapping(!wrapping);
    }

    config_.setListViewInBrowser(wrapping);
    setProperIconForViewButton();
}

void BrowseFilesWidget::currentTabChanged([[maybe_unused]] int index)
{
    FileExplorer* explorer = currentListView();
    if (!explorer->initialized())
        explorer->initialize();

    // Set current file path on tab switch.
    filePathLineEdit_->blockSignals(true);
    filePathLineEdit_->setText(explorer->getCurrentPath());
    filePathLineEdit_->blockSignals(false);
}
