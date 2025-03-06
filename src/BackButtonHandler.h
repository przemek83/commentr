#ifndef BACKBUTTONHANDLER_H
#define BACKBUTTONHANDLER_H

#include <QObject>

class BackButtonHandler : public QObject
{
Q_OBJECT
public:
    explicit BackButtonHandler(QObject* parent);

    virtual ~BackButtonHandler();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Q_DISABLE_COPY(BackButtonHandler)
};

#endif // BACKBUTTONHANDLER_H
