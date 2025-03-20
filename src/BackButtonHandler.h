#ifndef BACKBUTTONHANDLER_H
#define BACKBUTTONHANDLER_H

#include <QObject>

class BackButtonHandler : public QObject
{
    Q_OBJECT
public:
    explicit BackButtonHandler(QObject* parent);

    ~BackButtonHandler() override;

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    Q_DISABLE_COPY(BackButtonHandler)
};

#endif  // BACKBUTTONHANDLER_H
