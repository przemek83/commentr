#ifndef PROXYSTYLETEST_H
#define PROXYSTYLETEST_H

#include <QObject>

class ProxyStyleTest : public QObject
{
    Q_OBJECT

private slots:
    static void pixelMetricScrollBarUsesAdjustedScale();
    static void pixelMetricUiScaledMetricUsesAdjustedValue();
    static void pixelMetricUnrelatedMetricUsesBaseMetric();
};

#endif  // PROXYSTYLETEST_H
