#include <QTest>

#include "JavaHighlighterTest.h"
#include "SomeTest.h"

int main(int argc, char* argv[])
{
    const QApplication a(argc, argv);

    int status{EXIT_SUCCESS};
    SomeTest someTest;
    status |= QTest::qExec(&someTest);

    JavaHighlighterTest javaHighlighterTest;
    status |= QTest::qExec(&javaHighlighterTest);

    return status;
}
