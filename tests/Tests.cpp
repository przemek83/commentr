#include <QApplication>
#include <QTest>

#include "JavaHighlighterTest.h"
#include "PythonHighlighterTest.h"

int main(int argc, char* argv[])
{
    const QApplication a(argc, argv);

    int status{EXIT_SUCCESS};
    JavaHighlighterTest javaHighlighterTest;
    PythonHighlighterTest pythonHighlighterTest;

    status |= QTest::qExec(&javaHighlighterTest);
    status |= QTest::qExec(&pythonHighlighterTest);

    return status;
}
