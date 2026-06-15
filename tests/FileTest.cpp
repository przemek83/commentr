#include "FileTest.h"

#include <QFileInfo>
#include <QTest>

#include "File.h"

void FileTest::testConstructorAndAccessors()
{
    File f(Common::Source::LOCAL, QString("/path/to/name.txt"),
           QString("content"));
    QCOMPARE(f.source(), Common::Source::LOCAL);
    QCOMPARE(f.path(), QString("/path/to"));
    QCOMPARE(f.baseName(), QString("name"));
    QCOMPARE(f.suffix(), QString("txt"));
    QCOMPARE(f.content(), QString("content"));
}

void FileTest::testSetters()
{
    File f(Common::Source::LOCAL, QString("/p/b.suf"), QString("c"));
    f.setSource(Common::Source::NOT_SET);
    QCOMPARE(f.source(), Common::Source::NOT_SET);

    f.setFilePath(QString("/new/path/"));
    QCOMPARE(f.path(), QString("/new/path"));

    f.setFilePath(QString("base"));
    QCOMPARE(f.baseName(), QString("base"));

    f.setFilePath(QString(".cpp"));
    QCOMPARE(f.suffix(), QString("cpp"));

    f.clearContent();
    QCOMPARE(f.content(), QString(""));
}

void FileTest::testGetFilePath()
{
    File f(Common::Source::LOCAL, QString("/a/b/file.h"), QString());
    QCOMPARE(f.getFilePath(), QString("/a/b/file.h"));

    File f2(Common::Source::LOCAL, QString("/a/dotfile"), QString());
    QCOMPARE(f2.getFilePath(), QString("/a/dotfile"));
}

void FileTest::testNoDirectoryPath()
{
    File f(Common::Source::LOCAL, QString("file.txt"), QString("c"));
    QCOMPARE(f.path(), QString("."));
    QCOMPARE(f.baseName(), QString("file"));
    QCOMPARE(f.suffix(), QString("txt"));
}

void FileTest::testDotfileAndNoSuffix()
{
    File f(Common::Source::LOCAL, QString(".bashrc"), QString());
    const QFileInfo fi(QStringLiteral(".bashrc"));
    QCOMPARE(f.baseName(), fi.baseName());
    QCOMPARE(f.suffix(), fi.suffix());
}

void FileTest::testMultipleDotsBaseNameAndSuffix()
{
    File f(Common::Source::LOCAL, QString("archive.tar.gz"), QString());
    const QFileInfo fi(QStringLiteral("archive.tar.gz"));
    QCOMPARE(f.baseName(), fi.baseName());
    QCOMPARE(f.suffix(), fi.suffix());
}

void FileTest::testTrailingAndRepeatedSlashes()
{
    File f1(Common::Source::LOCAL, QString("/a/b/"), QString());
    QCOMPARE(f1.baseName(), QString(""));

    File f2(Common::Source::LOCAL, QString("/a//b///file.txt"), QString());
    const QFileInfo fi2(QStringLiteral("/a//b///file.txt"));
    QCOMPARE(f2.path(), fi2.path());
    QCOMPARE(f2.baseName(), fi2.baseName());
}

void FileTest::testEmptyPathBehavior()
{
    File f(Common::Source::LOCAL, QString(""), QString());
    QCOMPARE(f.getFilePath(), QString(""));
    QCOMPARE(f.path(), QString(""));
    QCOMPARE(f.baseName(), QString(""));
    QCOMPARE(f.suffix(), QString(""));
}
