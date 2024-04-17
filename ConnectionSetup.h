#ifndef CONNECTIONSETUP_H
#define CONNECTIONSETUP_H

#include <QFile>
#include <QNetworkConfigurationManager>
#include <QWidget>
#include <QNetworkSession>

#include "qftp.h"

class EnhancedLineEdit;

namespace Ui {
class ConnectionSetup;
}

class ConnectionSetup : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionSetup(QWidget *parent = 0);

    virtual ~ConnectionSetup();

public slots:
    virtual void setVisible(bool visible);

private:
    Ui::ConnectionSetup *ui;

    Q_DISABLE_COPY(ConnectionSetup)

    void setupLineEdits();

    EnhancedLineEdit* hostLineEdit_;

    EnhancedLineEdit* loginLineEdit_;

    EnhancedLineEdit* passwordLineEdit_;

signals:
    void accept();

    void cancel();

private slots:
    void on_okButton_clicked();

    void on_checkButton_clicked();

    void checkFinished(bool result);
    void on_saveInfoCheckBox_toggled(bool checked);
};

#endif // CONNECTIONSETUP_H
