#ifndef TESTCOMMON_H
#define TESTCOMMON_H

#include <QObject>

class QTemporaryFile;

class TestCommon : public QObject
{
    Q_OBJECT

    static QString prepareTestFile(QTemporaryFile& tmp, const QString& content);

private slots:
    static void testLoadFile();
    static void testSaveFile();
    static void testNormalizeFont();
    static void testMainWindow();
    static void testSampleGetFormat();
};

#endif  // TESTCOMMON_H
