#include "TestCommon.h"
#include <qtestcase.h>

#include <QFile>
#include <QFileInfo>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QTest>
#include <QTextStream>
#include <QWidget>

#include "Common.h"
#include "SyntaxElement.h"

QString TestCommon::prepareTestFile(QTemporaryFile& tmp, const QString& content)
{
    tmp.open();
    QTextStream out(&tmp);
    out << content;
    out.flush();
    const QString path{tmp.fileName()};
    tmp.close();
    return path;
}

void TestCommon::testLoadFile()
{
    QTemporaryFile tmp;
    const QString expectedContent{"hello_common"};
    const QString path{prepareTestFile(tmp, expectedContent)};

    const QString currentContent{Common::loadFile(path)};
    QCOMPARE(currentContent, expectedContent);
}

void TestCommon::testSaveFile()
{
    QTemporaryDir tmpDir;
    const QString expectedContent{QStringLiteral("saved_data")};
    const QString path{QDir(tmpDir.path()).filePath("out.txt")};
    Common::saveFile(path, expectedContent);
    const QString currentContent{Common::loadFile(path)};
    QCOMPARE(currentContent, expectedContent);
}

void TestCommon::testNormalizeFont()
{
    QCOMPARE(Common::normalizeFont(200.0F), 128.0F);
    QCOMPARE(Common::normalizeFont(0.0F), 1.0F);
    QCOMPARE(Common::normalizeFont(12.0F), 12.0F);
}

void TestCommon::testMainWindow()
{
    QWidget top;
    QWidget child(&top);
    QWidget grandchild(&child);

    QWidget* main{Common::getMainWindow(&grandchild)};
    QCOMPARE(main, &top);
}

void TestCommon::testSampleGetFormat()
{
    QTextCharFormat keyword{Common::getFormat(SyntaxElement::KEYWORD)};
    QCOMPARE(keyword.fontWeight(), QFont::Bold);
    QCOMPARE(keyword.foreground().color(), QColor(Qt::darkBlue));

    QTextCharFormat function{Common::getFormat(SyntaxElement::FUNCTION)};
    QCOMPARE(function.fontItalic(), true);
    QCOMPARE(function.foreground().color(), QColor(Qt::blue));
}
