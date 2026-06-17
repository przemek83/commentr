#include "TestConfig.h"

#include <QCoreApplication>
#include <QSettings>
#include <QTemporaryDir>
#include <QTest>

Config TestConfig::prepareTestConfig(const QString& lastPickedDir,
                                     const QStringList& recentFiles)
{
    Config config;
    config.setListViewInBrowser(false);
    config.setLastPickedDir(lastPickedDir);
    config.setFontSize(testFontSize_);
    config.setToolbarFileAdded(false);
    config.setToolbarKeyboardAdded(false);
    config.addFilePathToRecentFiles(recentFiles.last());
    config.addFilePathToRecentFiles(recentFiles.first());
    return config;
}

void TestConfig::initTestCase()
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QCoreApplication::setOrganizationName("commentr_test_org");
    QCoreApplication::setApplicationName("commentr_test_app");
}

void TestConfig::checkRecentFiles(const QStringList& current,
                                  const QStringList& expected)
{
    QVERIFY(current.size() == expected.size());

    for (int i{0}; i < current.size(); ++i)
        QCOMPARE(current.at(i), expected.at(i));
}

void TestConfig::testSaveAndLoad()
{
    QTemporaryDir tmpDir;
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope,
                       tmpDir.path());

    const QStringList recentFiles{QStringLiteral("/path/a"),
                                  QStringLiteral("/path/b")};

    const QString lastPickedDir{QStringLiteral("/tmp/testdir")};

    Config config{prepareTestConfig(lastPickedDir, recentFiles)};
    config.save();

    Config loadedConfig;
    QCOMPARE(loadedConfig.listViewInBrowser(), false);
    QCOMPARE(loadedConfig.lastPickedDir(), lastPickedDir);
    QCOMPARE(loadedConfig.fontSize(), testFontSize_);
    QCOMPARE(loadedConfig.toolbarFileAdded(), false);
    QCOMPARE(loadedConfig.toolbarKeyboardAdded(), false);

    checkRecentFiles(loadedConfig.getRecentFiles(), recentFiles);
}
