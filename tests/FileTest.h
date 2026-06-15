#ifndef FILETEST_H
#define FILETEST_H

#include <QObject>

class FileTest : public QObject
{
    Q_OBJECT

private slots:
    static void testConstructorAndAccessors();
    static void testSetters();
    static void testGetFilePath();
    static void testFilePathHelpers();
    static void testFileNameEdgeCases();
};

#endif  // FILETEST_H
