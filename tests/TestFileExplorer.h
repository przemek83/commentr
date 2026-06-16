#ifndef TESTFILEEXPLORER_H
#define TESTFILEEXPLORER_H

#include <QObject>

#include "Config.h"
#include "FileBrowser/FileExplorer.h"

class TestFileExplorer : public QObject
{
    Q_OBJECT

private:
    Config config_;

private slots:
    void initTestCase();
    void testFileIsValidOpenPositive();
    void testFileIsValidOpenNegative();
    void testPerformOperationOpen();
};

#endif  // TESTFILEEXPLORER_H
