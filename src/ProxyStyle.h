#ifndef PROXYSTYLE_H
#define PROXYSTYLE_H

#include <QProxyStyle>

class ProxyStyle : public QProxyStyle
{
public:
    ProxyStyle(const QString& style, float uiSize);
    ~ProxyStyle() override = default;

    int pixelMetric(PixelMetric metric, const QStyleOption* option,
                    const QWidget* widget) const override;

    QSize sizeFromContents(ContentsType type, const QStyleOption* option,
                           const QSize& size,
                           const QWidget* widget) const override;

    void drawPrimitive(PrimitiveElement element, const QStyleOption* option,
                       QPainter* painter, const QWidget* widget) const override;

private:
    Q_DISABLE_COPY_MOVE(ProxyStyle)

    int adjustSize(int size) const;

    float actualUisize_;
};

#endif  // PROXYSTYLE_H
