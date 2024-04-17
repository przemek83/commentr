#include <QApplication>
#include <QStyleOption>
#include <QDebug>

#include "ProxyStyle.h"
#include "Config.h"

int ProxyStyle::actualUisize_ = 36;

ProxyStyle::ProxyStyle(QString& name) : QProxyStyle(name)
{

}

ProxyStyle::~ProxyStyle()
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
        return QSize(defaultSize.width(), actualUisize_);
    }

    if( CT_PushButton == type )
    {
        return QSize(defaultSize.width(), qRound(actualUisize_ * 3.0 / 4.0));
    }

    //Lineedit.
    if( CT_LineEdit == type )
    {
        return QSize(defaultSize.width(), qRound(actualUisize_ * 2.0 / 3.0));
    }

//    if( CT_ToolButton == type  )
//    {
//        return QSize(qRound(actualUisize_ * 2.0 / 3.0),
//                     qRound(actualUisize_ * 2.0 / 3.0));
//    }

    //Checkbox.
    if( CT_CheckBox == type )
    {
        return QSize(defaultSize.width(),
                     qRound(actualUisize_ * 5.0 / 9.0));
    }

    return defaultSize;
}

int ProxyStyle::pixelMetric(PixelMetric metric,
                            const QStyleOption* option,
                            const QWidget* widget) const
{
    //Scroll bars width/height and tab bar scroll buttons.
    if( PM_ScrollBarExtent == metric ||
        PM_TabBarScrollButtonWidth == metric )
    {
        return qRound(actualUisize_ * 5.0 / 6.0);
    }

    //Toolbar handle and ">>" button.
    if( PM_ToolBarHandleExtent == metric ||
        PM_ToolBarExtensionExtent == metric/* ||
        PM_TabCloseIndicatorWidth == metric ||
        PM_TabCloseIndicatorHeight == metric */)
    {
        return qRound(actualUisize_ * 5.0 / 9.0);
    }

    //Toolbar buttons and icons in file browser.
    if( PM_ListViewIconSize == metric )
    {
        return qRound(actualUisize_ * 4.0 / 5.0);
    }

    if( PM_ToolBarIconSize == metric )
    {
        return actualUisize_;
    }

    if( PM_ButtonIconSize == metric )
    {
        return qRound(actualUisize_ * 3.0 / 4.0);
    }

    //CheckBox square dimensions.
    if( PM_IndicatorWidth == metric ||
        PM_IndicatorHeight == metric )
    {
        return qRound(actualUisize_ * 5.0 / 9.0);
    }

    return QProxyStyle::pixelMetric(metric, option, widget);
}

void ProxyStyle::updateUisize()
{
    actualUisize_ = Config::getInstance().uiSize();

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
