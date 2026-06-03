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
#include "SpellChecker.h"

namespace
{
void placeSamples()
{
    const QString samplesPath{QStringLiteral(":/samples/samples/")};
    const QStringList potentialPaths{QStandardPaths::standardLocations(
        QStandardPaths::AppLocalDataLocation)};
    for (const auto& path : potentialPaths)
    {
        const QFileInfo currentPath(path);
        if (currentPath.isDir() && (!currentPath.isWritable()))
            continue;

        const QDir allSamples(samplesPath);
        for (const auto& sample : allSamples.entryList())
        {
            QFile file(samplesPath + sample);
            file.copy(currentPath.absoluteFilePath() + QStringLiteral("/") +
                      sample);
        }

        return;
    }
}
}  // namespace

int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationName(QStringLiteral("Ble"));
    QCoreApplication::setApplicationName(QStringLiteral("CommentR"));

#ifdef Q_OS_WIN
    qputenv("QT_QPA_PLATFORM", "windows:darkmode=0");
#endif

    Config config;

    QApplication a(argc, argv);

    if (config.firstUse())
    {
        config.setFirstUse(false);
        config.setDefaultFont();
        placeSamples();
    }

    SpellChecker spellChecker;
    spellChecker.initDictionary(Common::loadFile(QStringLiteral(":/uk.dic")));

    MainWindow w(std::move(config), std::move(spellChecker));

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
