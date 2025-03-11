#include <QCoreApplication>
#include <QTest>

#include "SomeTest.h"

int main(int argc, char* argv[])
{
    const QCoreApplication a(argc, argv);

    SomeTest someTest;
    QTest::qExec(&someTest);

    return 0;
}
