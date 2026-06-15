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
    static void testNoDirectoryPath();
    static void testDotfileAndNoSuffix();
    static void testMultipleDotsBaseNameAndSuffix();
    static void testTrailingAndRepeatedSlashes();
    static void testEmptyPathBehavior();
};

#endif  // FILETEST_H
