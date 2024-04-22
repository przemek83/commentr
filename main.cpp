#include <QApplication>
#include <QSplashScreen>
#include <QScrollerProperties>
#include <QScroller>
#include <QDebug>
#include <QEasingCurve>
#include <QStyleFactory>

#include "MainWindow.h"
#include "SpellChecker.h"
#include "Config.h"
#include "ProxyStyle.h"
#include "Common.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Ble");
    QCoreApplication::setApplicationName("CommentR");

#ifdef Q_OS_WIN
    qputenv("QT_QPA_PLATFORM", "windows:darkmode=0");
#endif

    //Loads config on getInstance.
    ProxyStyle::updateUisize();

    QApplication a(argc, argv);

    //Can be used only after QApplication construction.
    if( true == Config::getInstance().firstUse() )
    {
        Config::getInstance().determineUiSize();
        Config::getInstance().setDefaultFont();
        ProxyStyle::updateUisize();
    }

//    //Do not allow size lower than 5 (historical reason).
//    if( Config::getInstance().uiSize() < 5 )
//    {
//        Config::getInstance().determineUiSize();
//        ProxyStyle::updateUisize();
//    }

    QObject::connect(qApp,
                     SIGNAL(aboutToQuit()),
                     &Config::getInstance(),
                     SLOT(save()));

    //QPixmap pixmap(":/splash.jpg");
//    QSplashScreen splash(pixmap);
//    splash.show();
//    splash.showMessage("Loading dictionary...");

    SpellChecker::getInstance().initDictionary(
        Common::loadFile(":/uk.dic"));

//    splash.showMessage("Showing main window...");

    MainWindow w;

//    splash.finish(&w);

#ifdef Q_OS_ANDROID
    w.setWindowFlags( Qt::FramelessWindowHint );
    w.setWindowState( w.windowState() | Qt::WindowFullScreen );
    w.setWindowModality( Qt::ApplicationModal );
    //If fullscreen used virtual keyboard do not resize window.
    //w.showFullScreen();
    w.show();
#else
    w.show();
#endif

#ifdef DEMO
    w.initDemo();
#endif

    return a.exec();
}
