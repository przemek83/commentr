#ifndef PROXYSTYLE_H
#define PROXYSTYLE_H

#include <QProxyStyle>

class ProxyStyle : public QProxyStyle
{
public:
    ProxyStyle(QString &name);

    virtual ~ProxyStyle();

    int pixelMetric(PixelMetric metric,
                    const QStyleOption* option,
                    const QWidget* widget) const;

    QSize sizeFromContents(ContentsType type,
                           const QStyleOption* option,
                           const QSize & contentsSize,
                           const QWidget * widget = 0) const;

    static void updateUisize();

    void drawPrimitive(PrimitiveElement element,
                       const QStyleOption* option,
                       QPainter* painter,
                       const QWidget* widget = 0) const;

private:
    Q_DISABLE_COPY(ProxyStyle)

    static int actualUisize_;
};

#endif // PROXYSTYLE_H
