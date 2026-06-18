#include "TestFileExplorer.h"

#include <QDir>
#include <QFile>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QTest>
#include <QTextStream>

#include "Common.h"
#include "File.h"

void TestFileExplorer::initTestCase() { config_ = Config(); }

void prepareFile(const QString& filePath, const QString& content)
{
    QFile file(filePath);
    QVERIFY(file.open(QIODevice::WriteOnly));
    QTextStream out(&file);
    out << content;
    file.close();
}

void TestFileExplorer::testFileIsValidOpenPositive()
{
    FileExplorer explorer(true, config_, nullptr);
    explorer.initialize();

    QTemporaryDir tmpDir;
    const QString filePath{QDir(tmpDir.path()).filePath("t.txt")};
    prepareFile(filePath, QStringLiteral("hello"));

    QVERIFY(explorer.fileIsValid(filePath));
}

void TestFileExplorer::testFileIsValidOpenNegative()
{
    FileExplorer explorer(true, config_, nullptr);
    explorer.initialize();

    QTemporaryDir tmpDir;
    const QString filePath{QDir(tmpDir.path()).filePath("t.txt")};
    prepareFile(filePath, QStringLiteral("hello"));

    const QString nonExist{QDir(tmpDir.path()).filePath("does_not_exist.txt")};
    QVERIFY(!explorer.fileIsValid(nonExist));
}

void TestFileExplorer::testPerformOperationOpen()
{
    FileExplorer explorer(true, config_, nullptr);
    explorer.initialize();

    QTemporaryDir tmpDir;
    const QString content{QStringLiteral("world")};
    const QString filePath{QDir(tmpDir.path()).filePath("t2.txt")};
    prepareFile(filePath, content);

    File capturedFile(Common::Source::NOT_SET, QStringLiteral(""),
                      QStringLiteral(""));
    bool invoked{false};

    qRegisterMetaType<File>("File");
    QObject::connect(&explorer, &FileExplorer::filePrepared,
                     [&](File f)
                     {
                         capturedFile = std::move(f);
                         invoked = true;
                     });

    explorer.performOperationOnFile(filePath);

    QTRY_VERIFY(invoked);
    QCOMPARE(capturedFile.getFilePath(), filePath);
    QCOMPARE(capturedFile.content(), content);
}
