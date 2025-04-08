#include <QApplication>
#include <QDir>
#include <QEasingCurve>
#include <QScroller>
#include <QScrollerProperties>
#include <QSplashScreen>
#include <QStandardPaths>
#include <QStyleFactory>

#include "Common.h"
#include "Config.h"
#include "MainWindow.h"
#include "ProxyStyle.h"
#include "SpellChecker.h"

namespace
{
void placeSamples()
{
    const QString samplesPath{":/samples/samples/"};
    const QStringList potentialPaths{QStandardPaths::standardLocations(
        QStandardPaths::AppLocalDataLocation)};
    for (const auto& path : potentialPaths)
    {
        const QFileInfo currentPath(path);
        if (currentPath.isDir() && !currentPath.isWritable())
            continue;

        const QDir allSamples(samplesPath);
        for (const auto& sample : allSamples.entryList())
        {
            QFile file(samplesPath + sample);
            file.copy(currentPath.absoluteFilePath() + "/" + sample);
        }

        return;
    }
}
}  // namespace

int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationName("Ble");
    QCoreApplication::setApplicationName("CommentR");

#ifdef Q_OS_WIN
    qputenv("QT_QPA_PLATFORM", "windows:darkmode=0");
#endif

    // Loads config on getInstance.
    ProxyStyle::updateUisize();

    QApplication a(argc, argv);

    // Can be used only after QApplication construction.
    if (Config::getInstance().firstUse())
    {
        Config::getInstance().setDefaultFont();
        ProxyStyle::updateUisize();
        placeSamples();
    }

    QObject::connect(qApp, SIGNAL(aboutToQuit()), &Config::getInstance(),
                     SLOT(save()));

    SpellChecker::getInstance().initDictionary(Common::loadFile(":/uk.dic"));

    MainWindow w;

#ifdef Q_OS_ANDROID
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.setWindowState(w.windowState() | Qt::WindowFullScreen);
    w.setWindowModality(Qt::ApplicationModal);
    w.show();
#else
    w.show();
#endif

    return QApplication::exec();
}
