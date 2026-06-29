#ifndef TESTCONFIG_H
#define TESTCONFIG_H

#include <QObject>

#include "Config.h"

class TestConfig : public QObject
{
    Q_OBJECT

private:
    static Config prepareTestConfig(const QString& lastPickedDir,
                                    const QStringList& recentFiles);
    static void checkRecentFiles(const QStringList& current,
                                 const QStringList& expected);

    static constexpr float testFontSize_{12.5F};

private slots:
    static void initTestCase();
    static void testSaveAndLoad();
};

#endif  // TESTCONFIG_H
