#ifndef PYTHONHIGHLIGHTERTEST_H
#define PYTHONHIGHLIGHTERTEST_H

#include <QObject>

class PythonHighlighterTest : public QObject
{
    Q_OBJECT

private slots:
    static void testKeywordHighlighting();
    static void testQuotationAndCommentHighlighting();
};

#endif  // PYTHONHIGHLIGHTERTEST_H
