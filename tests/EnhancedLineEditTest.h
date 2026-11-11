#ifndef ENHANCEDLINEEDITTEST_H
#define ENHANCEDLINEEDITTEST_H

#include <QObject>

class EnhancedLineEditTest : public QObject
{
    Q_OBJECT

private slots:
    static void constructorSetsMarginsFromUiSize();
    static void mouseMoveEventIsIgnored();
    static void styleChangeEventRefreshesMargins();
};

#endif  // ENHANCEDLINEEDITTEST_H
