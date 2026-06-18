#ifndef ENHANCEDLINEEDIT_H
#define ENHANCEDLINEEDIT_H

#include <QLineEdit>

class CursorPointerLineEdit;
class QGestureEvent;
class Config;

class EnhancedLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    EnhancedLineEdit(Config& config, QWidget* parent);

protected:
    void mouseReleaseEvent(QMouseEvent* e) override;

    void focusOutEvent(QFocusEvent* event) override;

    void resizeEvent(QResizeEvent* event) override;

    bool event(QEvent* event) override;

    void changeEvent(QEvent* event) override;

private:
    Q_DISABLE_COPY(EnhancedLineEdit)

    QPoint getPositionForVisualPointer() const;

    void resetPointerRange();

    void updateMarginSize();

    CursorPointerLineEdit* cursorPointer_;

    QWidget* mainWindow_;

    static constexpr int leftTextMargin_{5};

    int rightTextMargin_{10};

    /// Magic number built into Qt impacting text position.
    static constexpr int builtInTextMargin_{4};

    static constexpr int pointerHorizontalAdjustment_{6};

    Config& config_;

private slots:
    void textWasChanged(QString newText);

    void pointerMoved(QPoint pos);

    void matchPointerToCursorPosition();
};

#endif  // ENHANCEDLINEEDIT_H
