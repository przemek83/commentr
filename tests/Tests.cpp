#include <QApplication>
#include <QTest>

#include "CHighlighterTest.h"
#include "CSharpHighlighterTest.h"
#include "CplusPlusHighlighterTest.h"
#include "FileTest.h"
#include "HighlighterSpellCheckTest.h"
#include "JavaHighlighterTest.h"
#include "JavaScriptHighlighterTest.h"
#include "ObjectiveCHighlighterTest.h"
#include "PhpHighlighterTest.h"
#include "PythonHighlighterTest.h"
#include "SQLHighlighterTest.h"
#include "SpellCheckerTest.h"
#include "TestFileExplorer.h"
#include "VisualBasicHighlighterTest.h"

int main(int argc, char* argv[])
{
    const QApplication a(argc, argv);

    int status{EXIT_SUCCESS};
    JavaHighlighterTest javaHighlighterTest;
    status |= QTest::qExec(&javaHighlighterTest);

    PythonHighlighterTest pythonHighlighterTest;
    status |= QTest::qExec(&pythonHighlighterTest);

    CHighlighterTest cHighlighterTest;
    status |= QTest::qExec(&cHighlighterTest);

    CplusPlusHighlighterTest cPlusPlusHighlighterTest;
    status |= QTest::qExec(&cPlusPlusHighlighterTest);

    ObjectiveCHighlighterTest objectiveCHighlighterTest;
    status |= QTest::qExec(&objectiveCHighlighterTest);

    PhpHighlighterTest phpHighlighterTest;
    status |= QTest::qExec(&phpHighlighterTest);

    JavaScriptHighlighterTest javaScriptHighlighterTest;
    status |= QTest::qExec(&javaScriptHighlighterTest);

    CSharpHighlighterTest cSharpHighlighterTest;
    status |= QTest::qExec(&cSharpHighlighterTest);

    SQLHighlighterTest sqlHighlighterTest;
    status |= QTest::qExec(&sqlHighlighterTest);

    SpellCheckerTest spellCheckerTest;
    status |= QTest::qExec(&spellCheckerTest);

    HighlighterSpellCheckTest highlighterSpellCheckTest;
    status |= QTest::qExec(&highlighterSpellCheckTest);

    VisualBasicHighlighterTest visualBasicHighlighterTest;
    status |= QTest::qExec(&visualBasicHighlighterTest);

    FileTest fileTest;
    status |= QTest::qExec(&fileTest);

    TestFileExplorer fileExplorerTest;
    status |= QTest::qExec(&fileExplorerTest);

    return status;
}
