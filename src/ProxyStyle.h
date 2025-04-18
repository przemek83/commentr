#ifndef PROXYSTYLE_H
#define PROXYSTYLE_H

#include <QProxyStyle>

class ProxyStyle : public QProxyStyle
{
public:
    using QProxyStyle::QProxyStyle;

    int pixelMetric(PixelMetric metric, const QStyleOption* option,
                    const QWidget* widget) const override;

    QSize sizeFromContents(ContentsType type, const QStyleOption* option,
                           const QSize& size,
                           const QWidget* widget) const override;

    static void updateUisize();

    void drawPrimitive(PrimitiveElement element, const QStyleOption* option,
                       QPainter* painter,
                       const QWidget* widget = nullptr) const override;

private:
    Q_DISABLE_COPY(ProxyStyle)

    static int adjustSize(int size);

    static float actualUisize_;
};

#endif  // PROXYSTYLE_H
