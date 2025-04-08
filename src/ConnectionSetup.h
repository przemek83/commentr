#ifndef CONNECTIONSETUP_H
#define CONNECTIONSETUP_H

#include <QFile>
#include <QWidget>

class EnhancedLineEdit;

namespace Ui
{
class ConnectionSetup;
}

class ConnectionSetup : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionSetup(QWidget* parent = 0);

    ~ConnectionSetup() override;

public slots:
    void setVisible(bool visible) override;

private:
    Ui::ConnectionSetup* ui;

    Q_DISABLE_COPY(ConnectionSetup)

    void setupLineEdits();

    EnhancedLineEdit* hostLineEdit_{nullptr};

    EnhancedLineEdit* loginLineEdit_{nullptr};

    EnhancedLineEdit* passwordLineEdit_{nullptr};

signals:
    void accept();

    void cancel();

private slots:
    void on_okButton_clicked();

    void on_checkButton_clicked();

    void checkFinished(bool result);
    void on_saveInfoCheckBox_toggled(bool checked);
};

#endif  // CONNECTIONSETUP_H
