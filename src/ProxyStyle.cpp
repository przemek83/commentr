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
    int pixelMetric = QProxyStyle::pixelMetric(metric, option, widget);

    // Scroll bars width/height and tab bar scroll buttons.
    if (PM_ScrollBarExtent == metric || PM_TabBarScrollButtonWidth == metric)
        return ::qRound(pixelMetric * actualUisize_ * 5.0 / 6.0);

    // Toolbar handle and ">>" button.
    if (PM_ToolBarHandleExtent == metric || PM_ToolBarExtensionExtent == metric)
        return ::qRound(pixelMetric * actualUisize_);

    // Toolbar buttons and icons in file browser.
    if (PM_ListViewIconSize == metric)
        return ::qRound(pixelMetric * actualUisize_);

    if (PM_ToolBarIconSize == metric)
        return pixelMetric * actualUisize_;

    if (PM_ButtonIconSize == metric)
        return ::qRound(pixelMetric * actualUisize_);

    // CheckBox square dimensions.
    if (PM_IndicatorWidth == metric || PM_IndicatorHeight == metric)
        return ::qRound(pixelMetric * actualUisize_);

    return pixelMetric;
}

void ProxyStyle::updateUisize()
{
    actualUisize_ = Config::getInstance().uiSize() > 0
                        ? Config::getInstance().uiSize()
                        : actualUisize_;

    QString styleSetInConfig(Config::getInstance().style());
    qApp->setStyle(new ProxyStyle(styleSetInConfig));
}

void ProxyStyle::drawPrimitive(QStyle::PrimitiveElement element,
                               const QStyleOption* option, QPainter* painter,
                               const QWidget* widget) const
{
    // Do not show focus rectangle on item views.
    if (QStyleOption::SO_FocusRect == option->type &&
        QStyle::PE_FrameFocusRect == element)
    {
        return;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

int ProxyStyle::adjustSize(int size)
{
    return ::qRound(static_cast<float>(size) * actualUisize_);
}
