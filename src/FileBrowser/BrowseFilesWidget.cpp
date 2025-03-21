#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>

#include "../Common.h"
#include "../Config.h"
#include "../EnhancedLineEdit.h"
#include "../File.h"
#include "BrowseFilesWidget.h"
#include "Explorer.h"
#include "ExplorerFtp.h"
#include "ExplorerLocal.h"
#include "ui_BrowseFilesWidget.h"

BrowseFilesWidget::BrowseFilesWidget(bool open, QWidget* parent)
    : QWidget(parent), ui(new Ui::BrowseFilesWidget), filePathLineEdit_(nullptr)
{
    ui->setupUi(this);

    initLineEdit();

    ExplorerLocal* localListView = new ExplorerLocal(open, ui->tabWidget);

    connect(localListView, SIGNAL(filePrepared(File*)), this,
            SIGNAL(filePrepared(File*)));

    connect(localListView, SIGNAL(pathChanged(QString)), filePathLineEdit_,
            SLOT(setText(QString)));

    ui->tabWidget->insertTab(static_cast<int>(Common::SOURCE_LOCAL),
                             localListView,
                             // QIcon(":Icons/close.png"),
                             "local");

#ifdef FTP
    ExplorerFtp* ftp = new ExplorerFtp(open, ui->tabWidget);

    connect(ftp, SIGNAL(filePrepared(File*)), this,
            SIGNAL(filePrepared(File*)));

    connect(ftp, SIGNAL(pathChanged(QString)), filePathLineEdit_,
            SLOT(setText(QString)));

    ui->tabWidget->insertTab(static_cast<int>(Common::SOURCE_FTP), ftp,
                             // QIcon(":Icons/close.png"),
                             "ftp");
#endif

    setProperIconForViewButton();

    // Set line edit text, do not allow emitting signals
    //(virtual methods can not be called).
    filePathLineEdit_->blockSignals(true);
    filePathLineEdit_->setText(localListView->getCurrentPath());
    filePathLineEdit_->blockSignals(false);

    setAttribute(Qt::WA_AcceptTouchEvents);

    // Needed?
    // updateGeometry();
}

BrowseFilesWidget::~BrowseFilesWidget() { delete ui; }

void BrowseFilesWidget::initLineEdit()
{
    filePathLineEdit_ = new EnhancedLineEdit(this);
    ui->upperHorizontalLayout->insertWidget(1, filePathLineEdit_);
    filePathLineEdit_->setStyleSheet("QLineEdit{background: #FFBFBF;}");

    connect(filePathLineEdit_, SIGNAL(returnPressed()), this,
            SLOT(filePathReturnPressed()));

    connect(filePathLineEdit_, SIGNAL(textChanged(QString)), this,
            SLOT(filePathTextChanged(QString)));

    filePathLineEdit_->setInputMethodHints(Qt::ImhNoPredictiveText);
}

void BrowseFilesWidget::filePathReturnPressed()
{
    QString filePath(filePathLineEdit_->text());
    switch (static_cast<Common::Source>(ui->tabWidget->currentIndex()))
    {
        case Common::SOURCE_LOCAL:
        {
            QFileInfo fileInfo(filePath);

            QString canonicalFilePath(fileInfo.canonicalFilePath());
            if (true == fileInfo.isDir())
            {
                currentListView()->setPath(canonicalFilePath);
                return;
            }

            currentListView()->performOperationOnFile(filePath);

            break;
        }

        case Common::SOURCE_FTP:
        {
            QString path(File::filePathToPath(filePath));
            if (true == File::filePathToFileName(filePath).isEmpty() &&
                currentListView()->getCurrentPath() != path)
            {
                currentListView()->setPath(path);
                return;
            }

            currentListView()->performOperationOnFile(filePath);

            break;
        }

        default:
        {
            Q_ASSERT(false);
            break;
        }
    }
}

void BrowseFilesWidget::filePathTextChanged(const QString& arg1)
{
    bool valid = currentListView()->fileIsValid(arg1);
    if (true == valid)
    {
        filePathLineEdit_->setStyleSheet("QLineEdit{background: white;}");
    }
    else
    {
        filePathLineEdit_->setStyleSheet("QLineEdit{background: #FFBFBF;}");
    }

    if (true == filePathLineEdit_->text().isEmpty())
    {
        currentListView()->setPath("");
        filePathLineEdit_->setText(Common::rootPath());
    }
}

void BrowseFilesWidget::setProperIconForViewButton()
{
    if (false == currentListView()->isWrapping())
    {
        QIcon viewIcon =
            QApplication::style()->standardIcon(QStyle::SP_FileDialogListView);
        ui->changeView->setIcon(viewIcon);
    }
    else
    {
        QIcon viewIcon = QApplication::style()->standardIcon(
            QStyle::SP_FileDialogDetailedView);
        ui->changeView->setIcon(viewIcon);
    }
}

Explorer* BrowseFilesWidget::currentListView()
{
    return dynamic_cast<Explorer*>(ui->tabWidget->currentWidget());
}

void BrowseFilesWidget::on_changeView_clicked()
{
    bool wrapping = currentListView()->isWrapping();
    int tabCount = ui->tabWidget->count();
    for (int i = 0; i < tabCount; ++i)
    {
        QListView* listView =
            dynamic_cast<QListView*>(ui->tabWidget->widget(i));

        listView->setWrapping(!wrapping);
    }

    Config::getInstance().setListViewInBrowser(wrapping);
    setProperIconForViewButton();
}

void BrowseFilesWidget::on_tabWidget_currentChanged(int index)
{
    if (false == isFtpConfigured(index))
    {
        return;
    }

    Explorer* explorer = currentListView();
    if (explorer != nullptr && false == explorer->initialized())
    {
        explorer->initialize();
    }

    // filePathLineEdit_->setReadOnly(index ==
    // static_cast<int>(Common::SOURCE_FTP));
    filePathLineEdit_->setVisible(index !=
                                  static_cast<int>(Common::SOURCE_FTP));

    // Set current file path on tab switch.
    filePathLineEdit_->blockSignals(true);
    filePathLineEdit_->setText(explorer->getCurrentPath());
    filePathLineEdit_->blockSignals(false);
}

bool BrowseFilesWidget::isFtpConfigured(int index)
{
    if (index == static_cast<int>(Common::SOURCE_FTP) &&
        Config::getInstance().ftpHost().isEmpty())
    {
        QMessageBox::StandardButton answer = QMessageBox::question(
            this, tr("FTP"), tr("FTP not configured. Configure now?"));

        if (QMessageBox::Yes == answer)
        {
            emit configureFtpConnection();
        }
        else
        {
            ui->tabWidget->setCurrentIndex(
                static_cast<int>(Common::SOURCE_LOCAL));
        }

        return false;
    }
    return true;
}
