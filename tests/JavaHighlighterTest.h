#ifndef JAVAHIGHLIGHTERTEST_H
#define JAVAHIGHLIGHTERTEST_H

#include <QObject>

class JavaHighlighterTest : public QObject
{
    Q_OBJECT

private slots:
    static void testKeywordHighlighting();
    static void testQuotationAndCommentHighlighting();
};

#endif  // JAVAHIGHLIGHTERTEST_H
