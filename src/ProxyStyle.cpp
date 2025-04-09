#include "ProxyStyle.h"

#include <QApplication>
#include <QStyleOption>

#include "Config.h"

float ProxyStyle::actualUisize_ = 1.;

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

int ProxyStyle::pixelMetric(PixelMetric metric, const QStyleOption* option,
                            const QWidget* widget) const
{
    int pixelMetric{QProxyStyle::pixelMetric(metric, option, widget)};

    if ((metric == PM_ScrollBarExtent) ||
        (metric == PM_TabBarScrollButtonWidth))
        return ::qRound(adjustSize(pixelMetric) * 5.0 / 6.0);

    const bool isToolBarExtent{(metric == PM_ToolBarHandleExtent) ||
                               (metric == PM_ToolBarExtensionExtent)};
    const bool isCheckBoxIndicator{(metric == PM_IndicatorWidth) ||
                                   (metric == PM_IndicatorHeight)};

    if (isToolBarExtent || isCheckBoxIndicator ||
        metric == PM_ListViewIconSize || metric == PM_ToolBarIconSize ||
        metric == PM_ButtonIconSize)
        return adjustSize(pixelMetric);

    return pixelMetric;
}

void ProxyStyle::updateUisize()
{
    if (Config::getInstance().uiSize() > 0)
        actualUisize_ = Config::getInstance().uiSize();

    QString styleSetInConfig(Config::getInstance().style());
    QApplication::setStyle(new ProxyStyle(styleSetInConfig));
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

int ProxyStyle::adjustSize(int size)
{
    return ::qRound(static_cast<float>(size) * actualUisize_);
}
