#include <QApplication>
#include <QTest>

#include "CHighlighterTest.h"
#include "CPlusPlusHighlighterTest.h"
#include "CSharpHighlighterTest.h"
#include "FileTest.h"
#include "HighlighterSpellCheckTest.h"
#include "JavaHighlighterTest.h"
#include "JavaScriptHighlighterTest.h"
#include "ObjectiveCHighlighterTest.h"
#include "PhpHighlighterTest.h"
#include "PythonHighlighterTest.h"
#include "SQLHighlighterTest.h"
#include "SpellCheckerTest.h"
#include "TestCommon.h"
#include "TestConfig.h"
#include "TestFileExplorer.h"
#include "VisualBasicHighlighterTest.h"

int main(int argc, char* argv[])
{
    const QApplication a(argc, argv);

    int status{EXIT_SUCCESS};
    JavaHighlighterTest javaHighlighterTest;
    status = std::max(status, QTest::qExec(&javaHighlighterTest));

    PythonHighlighterTest pythonHighlighterTest;
    status = std::max(status, QTest::qExec(&pythonHighlighterTest));

    CHighlighterTest cHighlighterTest;
    status = std::max(status, QTest::qExec(&cHighlighterTest));

    CPlusPlusHighlighterTest cPlusPlusHighlighterTest;
    status = std::max(status, QTest::qExec(&cPlusPlusHighlighterTest));

    ObjectiveCHighlighterTest objectiveCHighlighterTest;
    status = std::max(status, QTest::qExec(&objectiveCHighlighterTest));

    PhpHighlighterTest phpHighlighterTest;
    status = std::max(status, QTest::qExec(&phpHighlighterTest));

    JavaScriptHighlighterTest javaScriptHighlighterTest;
    status = std::max(status, QTest::qExec(&javaScriptHighlighterTest));

    CSharpHighlighterTest cSharpHighlighterTest;
    status = std::max(status, QTest::qExec(&cSharpHighlighterTest));

    SQLHighlighterTest sqlHighlighterTest;
    status = std::max(status, QTest::qExec(&sqlHighlighterTest));

    SpellCheckerTest spellCheckerTest;
    status = std::max(status, QTest::qExec(&spellCheckerTest));

    HighlighterSpellCheckTest highlighterSpellCheckTest;
    status = std::max(status, QTest::qExec(&highlighterSpellCheckTest));

    VisualBasicHighlighterTest visualBasicHighlighterTest;
    status = std::max(status, QTest::qExec(&visualBasicHighlighterTest));

    FileTest fileTest;
    status = std::max(status, QTest::qExec(&fileTest));

    TestFileExplorer fileExplorerTest;
    status = std::max(status, QTest::qExec(&fileExplorerTest));

    TestConfig testConfig;
    status = std::max(status, QTest::qExec(&testConfig));

    TestCommon testCommon;
    status = std::max(status, QTest::qExec(&testCommon));

    return status;
}
