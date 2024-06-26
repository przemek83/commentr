#include <QApplication>
#include <QStyleOption>
#include <QDebug>

#include "ProxyStyle.h"
#include "Config.h"

float ProxyStyle::actualUisize_ = 1.;

ProxyStyle::ProxyStyle(QString& name) : QProxyStyle(name)
{

}

QSize ProxyStyle::sizeFromContents(ContentsType type,
                                   const QStyleOption* option,
                                   const QSize & contentsSize,
                                   const QWidget * widget) const
{

    QSize defaultSize =
        QProxyStyle::sizeFromContents(type, option, contentsSize, widget);

    //Items in browsers and buttons.
    if( CT_ItemViewItem == type )
    {
        return QSize(defaultSize.width(), defaultSize.height() * actualUisize_);
    }

    if( CT_PushButton == type )
    {
        return QSize(defaultSize.width(), qRound(defaultSize.height() * actualUisize_));
    }

    //Lineedit.
    if( CT_LineEdit == type )
    {
        return QSize(defaultSize.width(), qRound(defaultSize.height() * actualUisize_));
    }

//    if( CT_ToolButton == type  )
//    {
//        return QSize(qRound(actualUisize_ * 2.0 / 3.0),
//                     qRound(actualUisize_ * 2.0 / 3.0));
//    }

    //Checkbox.
    if( CT_CheckBox == type )
    {
        return QSize(defaultSize.width(), qRound(defaultSize.height() * actualUisize_));
    }

    return defaultSize;
}

int ProxyStyle::pixelMetric(PixelMetric metric,
                            const QStyleOption* option,
                            const QWidget* widget) const
{
    int pixelMetric = QProxyStyle::pixelMetric(metric, option, widget);

    //Scroll bars width/height and tab bar scroll buttons.
    if( PM_ScrollBarExtent == metric ||
        PM_TabBarScrollButtonWidth == metric )
    {
        return qRound(pixelMetric * actualUisize_ * 5.0 / 6.0);
    }

    //Toolbar handle and ">>" button.
    if( PM_ToolBarHandleExtent == metric ||
        PM_ToolBarExtensionExtent == metric/* ||
        PM_TabCloseIndicatorWidth == metric ||
        PM_TabCloseIndicatorHeight == metric */)
    {
        return qRound(pixelMetric * actualUisize_);
    }

    //Toolbar buttons and icons in file browser.
    if( PM_ListViewIconSize == metric )
    {
        return qRound(pixelMetric * actualUisize_);
    }

    if( PM_ToolBarIconSize == metric )
    {
        return pixelMetric * actualUisize_;
    }

    if( PM_ButtonIconSize == metric )
    {
        return qRound(pixelMetric * actualUisize_);
    }

    //CheckBox square dimensions.
    if (PM_IndicatorWidth == metric || PM_IndicatorHeight == metric) {
        return qRound(pixelMetric * actualUisize_);
    }

    return pixelMetric;
}

void ProxyStyle::updateUisize()
{
    actualUisize_ = Config::getInstance().uiSize() > 0 ? Config::getInstance().uiSize()
                                                       : actualUisize_;

    QString styleSetInConfig(Config::getInstance().style());
    qApp->setStyle(new ProxyStyle(styleSetInConfig));
}

void ProxyStyle::drawPrimitive(QStyle::PrimitiveElement element,
                               const QStyleOption* option,
                               QPainter* painter,
                               const QWidget* widget) const
{
    //Do not show focus rectangle on item views.
    if( QStyleOption::SO_FocusRect == option->type &&
        QStyle::PE_FrameFocusRect == element )
    {
        return;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
