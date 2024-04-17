#include <QDebug>
#include <QNetworkConfiguration>
#include <QSettings>
#include <QNetworkSession>
#include <QMessageBox>
#include <QRegExpValidator>

#include "ConnectionSetup.h"
#include "ui_ConnectionSetup.h"
#include "Config.h"
#include "FtpFileSaver.h"
#include "EnhancedLineEdit.h"

ConnectionSetup::ConnectionSetup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionSetup),
    hostLineEdit_(NULL),
    loginLineEdit_(NULL),
    passwordLineEdit_(NULL)
{
    ui->setupUi(this);

    setupLineEdits();

    ui->saveInfoCheckBox->blockSignals(true);
    ui->saveInfoCheckBox->setChecked(Config::getInstance().saveFtpPassword());
    ui->saveInfoCheckBox->blockSignals(false);

    connect(ui->cancelButton, SIGNAL(clicked()), this, SIGNAL(cancel()));
}

ConnectionSetup::~ConnectionSetup()
{
    delete ui;
}

void ConnectionSetup::setVisible(bool visible)
{
    static const Config& instance = Config::getInstance();
    hostLineEdit_->setText(instance.ftpHost());
    loginLineEdit_->setText(instance.ftpLogin());
    passwordLineEdit_->setText(instance.ftpPassword());

//    hostLineEdit_->setText("test.rebex.net");
//    loginLineEdit_->setText("demo");
//    passwordLineEdit_->setText("password");

    QWidget::setVisible(visible);
}

void ConnectionSetup::setupLineEdits()
{
    hostLineEdit_ = new EnhancedLineEdit(this);
    hostLineEdit_->setPlaceholderText("example.com");
    QRegularExpression hostRegExp(QRegularExpression("([^\\s/?\\.#-]+\\.?)+(/[^\\s]*)?$@iS"));
    hostLineEdit_->setValidator(new QRegExpValidator(hostRegExp, hostLineEdit_));
    hostLineEdit_->setInputMethodHints(Qt::ImhNoPredictiveText);
    ui->verticalLayout->insertWidget(2, hostLineEdit_);

    loginLineEdit_ = new EnhancedLineEdit(this);
    loginLineEdit_->setInputMethodHints(Qt::ImhNoPredictiveText);
    ui->verticalLayout->insertWidget(4, loginLineEdit_);

    passwordLineEdit_ = new EnhancedLineEdit(this);
    passwordLineEdit_->setInputMethodHints(Qt::ImhNoPredictiveText);
    ui->verticalLayout->insertWidget(6, passwordLineEdit_);
}

void ConnectionSetup::on_okButton_clicked()
{
    static Config& instance = Config::getInstance();
    instance.setFtpHost(hostLineEdit_->text());
    instance.setFtpLogin(loginLineEdit_->text());

    bool savePasswordChecked = ui->saveInfoCheckBox->isChecked();
    if( true == savePasswordChecked )
    {
        instance.setFtpPassword(passwordLineEdit_->text());
    }
    else
    {
        instance.setFtpPassword("");
    }

    Config::getInstance().setSaveFtpPassword(savePasswordChecked);

    emit accept();
}

void ConnectionSetup::on_checkButton_clicked()
{
    ui->checkButton->setEnabled(false);

    FtpFileSaver* ftpChecker = new FtpFileSaver(this);

    connect(ftpChecker,
            SIGNAL(operationFinished(bool)),
            this,
            SLOT(checkFinished(bool)));

    ftpChecker->checkConnection(hostLineEdit_->text(),
                                loginLineEdit_->text(),
                                passwordLineEdit_->text());
}

void ConnectionSetup::checkFinished(bool result)
{
    sender()->deleteLater();
    if( true == result )
    {
        QMessageBox::information(this, "FTP", "Connection OK.");
    }

    ui->checkButton->setEnabled(true);
}

void ConnectionSetup::on_saveInfoCheckBox_toggled(bool checked)
{
    if( true == checked )
    {
        QString msg = tr("Password will be saved as plain text. Save?");
        QMessageBox::StandardButton answer =
            QMessageBox::question(this,
                                  tr("Confirm"),
                                  msg);

        if( QMessageBox::No == answer )
        {
            ui->saveInfoCheckBox->setChecked(false);
        }
    }
}
