#include "ProxyStyleTest.h"

#include <QProxyStyle>
#include <QStyleFactory>
#include <QTest>

#include "ProxyStyle.h"

namespace
{
QString availableStyleName()
{
    const QStringList styles{QStyleFactory::keys()};

    if (styles.contains(QStringLiteral("Fusion")))
        return QStringLiteral("Fusion");

    if (styles.isEmpty())
        return {};

    return styles.first();
}
}  // namespace

void ProxyStyleTest::pixelMetricScrollBarUsesAdjustedScale()
{
    const QString styleName{availableStyleName()};

    const QProxyStyle baseStyle(styleName);
    const int baseMetric{
        baseStyle.pixelMetric(QStyle::PM_ScrollBarExtent, nullptr, nullptr)};
    const int expected{
        ::qRound(static_cast<float>(baseMetric) * 2.0F * (5.0 / 6.0))};

    const ProxyStyle proxyStyle(styleName, 2.0F);
    const int actual{
        proxyStyle.pixelMetric(QStyle::PM_ScrollBarExtent, nullptr, nullptr)};
    QCOMPARE(actual, expected);
}

void ProxyStyleTest::pixelMetricUiScaledMetricUsesAdjustedValue()
{
    const QString styleName{availableStyleName()};

    const QProxyStyle baseStyle(styleName);
    const int baseMetric{baseStyle.pixelMetric(QStyle::PM_ToolBarHandleExtent,
                                               nullptr, nullptr)};
    const int expected{::qRound(static_cast<float>(baseMetric) * 1.5F)};

    const ProxyStyle proxyStyle(styleName, 1.5F);
    const int actual{proxyStyle.pixelMetric(QStyle::PM_ToolBarHandleExtent,
                                            nullptr, nullptr)};
    QCOMPARE(actual, expected);
}

void ProxyStyleTest::pixelMetricUnrelatedMetricUsesBaseMetric()
{
    const QString styleName{availableStyleName()};

    const QProxyStyle baseStyle(styleName);
    const int baseMetric{
        baseStyle.pixelMetric(QStyle::PM_DefaultFrameWidth, nullptr, nullptr)};

    const ProxyStyle proxyStyle(styleName, 2.0F);
    const int actual{
        proxyStyle.pixelMetric(QStyle::PM_DefaultFrameWidth, nullptr, nullptr)};
    QCOMPARE(actual, baseMetric);
}
