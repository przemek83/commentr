#ifndef BACKBUTTONHANDLER_H
#define BACKBUTTONHANDLER_H

#include <QObject>

class BackButtonHandler : public QObject
{
public:
    using QObject::QObject;

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif  // BACKBUTTONHANDLER_H
