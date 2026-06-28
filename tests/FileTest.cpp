#include "FileTest.h"

#include <QFileInfo>
#include <QTest>

#include "File.h"

void FileTest::testConstructorAndAccessors()
{
    File f(Common::Source::LOCAL, QStringLiteral("/path/to/name.txt"),
           QStringLiteral("content"));
    QCOMPARE(f.source(), Common::Source::LOCAL);
    QCOMPARE(f.path(), QString("/path/to"));
    QCOMPARE(f.getFileName(), QString("name.txt"));
    QCOMPARE(f.suffix(), QString("txt"));
    QCOMPARE(f.content(), QString("content"));
}

void FileTest::testSetters()
{
    File f(Common::Source::LOCAL, QStringLiteral("/p/b.suf"), QStringLiteral("c"));
    f.setSource(Common::Source::NOT_SET);
    QCOMPARE(f.source(), Common::Source::NOT_SET);

    f.setFilePath(QStringLiteral("/new/path/"));
    QCOMPARE(f.path(), QString("/new/path"));

    f.setFilePath(QStringLiteral("base"));
    QCOMPARE(f.getFileName(), QString("base"));

    f.setFilePath(QStringLiteral(".cpp"));
    QCOMPARE(f.suffix(), QString("cpp"));

    f.clearContent();
    QCOMPARE(f.content(), QLatin1String(""));
}

void FileTest::testGetFilePath()
{
    File f(Common::Source::LOCAL, QStringLiteral("/a/b/file.h"), QString());
    QCOMPARE(f.getFilePath(), QString("/a/b/file.h"));

    File f2(Common::Source::LOCAL, QStringLiteral("/a/dotfile"), QString());
    QCOMPARE(f2.getFilePath(), QString("/a/dotfile"));
}

void FileTest::testNoDirectoryPath()
{
    File f(Common::Source::LOCAL, QStringLiteral("file.txt"), QStringLiteral("c"));
    QCOMPARE(f.path(), QString("."));
    QCOMPARE(f.getFileName(), QString("file.txt"));
    QCOMPARE(f.suffix(), QString("txt"));
}

void FileTest::testDotfileAndNoSuffix()
{
    File f(Common::Source::LOCAL, QStringLiteral(".bashrc"), QString());
    const QFileInfo fi(QStringLiteral(".bashrc"));
    QCOMPARE(f.getFileName(), fi.fileName());
    QCOMPARE(f.suffix(), fi.suffix());
}

void FileTest::testMultipleDotsBaseNameAndSuffix()
{
    File f(Common::Source::LOCAL, QStringLiteral("archive.tar.gz"), QString());
    const QFileInfo fi(QStringLiteral("archive.tar.gz"));
    QCOMPARE(f.getFileName(), fi.fileName());
    QCOMPARE(f.suffix(), fi.suffix());
}

void FileTest::testTrailingAndRepeatedSlashes()
{
    File f1(Common::Source::LOCAL, QStringLiteral("/a/b/"), QString());
    QCOMPARE(f1.getFileName(), QLatin1String(""));

    File f2(Common::Source::LOCAL, QStringLiteral("/a//b///file.txt"), QString());
    const QFileInfo fi2(QStringLiteral("/a//b///file.txt"));
    QCOMPARE(f2.path(), fi2.path());
    QCOMPARE(f2.getFileName(), fi2.fileName());
}

void FileTest::testEmptyPathBehavior()
{
    File f(Common::Source::LOCAL, QLatin1String(""), QString());
    QCOMPARE(f.getFilePath(), QLatin1String(""));
    QCOMPARE(f.path(), QLatin1String(""));
    QCOMPARE(f.getFileName(), QLatin1String(""));
    QCOMPARE(f.suffix(), QLatin1String(""));
}
