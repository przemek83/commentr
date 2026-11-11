#ifndef BACKBUTTONHANDLERTEST_H
#define BACKBUTTONHANDLERTEST_H

#include <QObject>

class BackButtonHandlerTest : public QObject
{
    Q_OBJECT

private slots:
    static void keyReleaseBackClearsFocusAndStopsEvent();
    static void nonBackKeyPassesThrough();
};

#endif  // BACKBUTTONHANDLERTEST_H
