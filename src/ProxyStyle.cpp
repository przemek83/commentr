#include "ProxyStyle.h"

#include <QApplication>
#include <QStyleOption>

ProxyStyle::ProxyStyle(const QString& style, float uiSize)
    : QProxyStyle(style), actualUiSize_(uiSize)
{
}

QSize ProxyStyle::sizeFromContents(ContentsType type,
                                   const QStyleOption* option,
                                   const QSize& size,
                                   const QWidget* widget) const
{
    const QSize defaultSize{
        QProxyStyle::sizeFromContents(type, option, size, widget)};

    // Items in browsers and buttons.
    if (type == CT_ItemViewItem)
        return {defaultSize.width(), adjustSize(defaultSize.height())};

    if ((type == CT_PushButton) || (type == CT_LineEdit) ||
        (type == CT_CheckBox))

        return {defaultSize.width(), adjustSize(defaultSize.height())};

    return defaultSize;
}

bool ProxyStyle::isScrollBarMetric(QStyle::PixelMetric metric)
{
    return metric == QStyle::PM_ScrollBarExtent ||
           metric == QStyle::PM_TabBarScrollButtonWidth;
}

bool ProxyStyle::isUiScaledMetric(QStyle::PixelMetric metric)
{
    const bool isToolBarExtent{(metric == PM_ToolBarHandleExtent) ||
                               (metric == PM_ToolBarExtensionExtent)};
    const bool isCheckBoxIndicator{(metric == PM_IndicatorWidth) ||
                                   (metric == PM_IndicatorHeight)};
    const bool isIconSizeMetric{(metric == PM_ListViewIconSize) ||
                                (metric == PM_ToolBarIconSize) ||
                                (metric == PM_ButtonIconSize)};

    return isToolBarExtent || isCheckBoxIndicator || isIconSizeMetric;
}

int ProxyStyle::pixelMetric(PixelMetric metric, const QStyleOption* option,
                            const QWidget* widget) const
{
    const int baseMetric{QProxyStyle::pixelMetric(metric, option, widget)};

    if (isScrollBarMetric(metric))
    {
        constexpr double numerator{5.0};
        constexpr double denominator{6.0};
        return ::qRound(adjustSize(baseMetric) * (numerator / denominator));
    }

    if (isUiScaledMetric(metric))
        return adjustSize(baseMetric);

    return baseMetric;
}

void ProxyStyle::drawPrimitive(QStyle::PrimitiveElement element,
                               const QStyleOption* option, QPainter* painter,
                               const QWidget* widget) const
{
    // Do not show focus rectangle on item views.
    if ((option->type == QStyleOption::SO_FocusRect) &&
        (element == QStyle::PE_FrameFocusRect))
        return;

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

int ProxyStyle::adjustSize(int size) const
{
    return ::qRound(static_cast<float>(size) * actualUiSize_);
}
