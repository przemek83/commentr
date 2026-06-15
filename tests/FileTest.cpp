#include "FileTest.h"

#include <QTest>

#include "../src/File.h"

void FileTest::testConstructorAndAccessors()
{
    File f(Common::Source::LOCAL, QString("/path/to"), QString("name"),
           QString("txt"), QString("content"));
    QCOMPARE(f.source(), Common::Source::LOCAL);
    QCOMPARE(f.path(), QString("/path/to"));
    QCOMPARE(f.baseName(), QString("name"));
    QCOMPARE(f.suffix(), QString("txt"));
    QCOMPARE(f.content(), QString("content"));
}

void FileTest::testSetters()
{
    File f(Common::Source::LOCAL, QString("/p"), QString("b"), QString("suf"),
           QString("c"));
    f.setSource(Common::Source::NOT_SET);
    QCOMPARE(f.source(), Common::Source::NOT_SET);

    f.setPath(QString("/new/path"));
    QCOMPARE(f.path(), QString("/new/path"));

    f.setBaseName(QString("base"));
    QCOMPARE(f.baseName(), QString("base"));

    f.setSuffix(QString("cpp"));
    QCOMPARE(f.suffix(), QString("cpp"));

    f.clearContent();
    QCOMPARE(f.content(), QString(""));
}

void FileTest::testGetFilePath()
{
    File f(Common::Source::LOCAL, QString("/a/b"), QString("file"),
           QString("h"), QString());
    QCOMPARE(f.getFilePath(), QString("/a/b/file.h"));

    File f2(Common::Source::LOCAL, QString("/a"), QString("dotfile"),
            QString(""), QString());
    QCOMPARE(f2.getFilePath(), QString("/a/dotfile"));
}

void FileTest::testFilePathHelpers()
{
    QCOMPARE(File::filePathToPath(QString("/a/b/c.txt")), QString("/a/b"));
    QCOMPARE(File::filePathToFileName(QString("/a/b/c.txt")), QString("c.txt"));
    QCOMPARE(File::filePathToBaseName(QString("/a/b/c.txt")), QString("c"));
    QCOMPARE(File::filePathToSuffix(QString("/a/b/c.txt")), QString("txt"));
}

void FileTest::testFileNameEdgeCases()
{
    QCOMPARE(File::fileNameToBaseName(QString(".bashrc")), QString(".bashrc"));
    QCOMPARE(File::fileNameToSuffix(QString(".bashrc")), QString(""));

    QCOMPARE(File::fileNameToBaseName(QString("archive.tar.gz")),
             QString("archive.tar"));
    QCOMPARE(File::fileNameToSuffix(QString("archive.tar.gz")), QString("gz"));

    QCOMPARE(File::fileNameToBaseName(QString("file")), QString("file"));
    QCOMPARE(File::fileNameToSuffix(QString("file")), QString(""));
}
