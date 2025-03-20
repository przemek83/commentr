#ifndef ENHANCEDLINEEDIT_H
#define ENHANCEDLINEEDIT_H

#include <QLineEdit>

class CursorPointerLineEdit;
class QGestureEvent;

class EnhancedLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit EnhancedLineEdit(QWidget* parent = 0);

    ~EnhancedLineEdit() override;

protected:
    void mouseReleaseEvent(QMouseEvent* e) override;

    void focusOutEvent(QFocusEvent* event) override;

    void resizeEvent(QResizeEvent* event) override;

    bool event(QEvent* event) override;

    void changeEvent(QEvent* event) override;

private:
    Q_DISABLE_COPY(EnhancedLineEdit)

    /// Visual cursor.
    CursorPointerLineEdit* cursorPointer_;

    QWidget* mainWindow_;

    QWidget* getMainWindow();

    QPoint getPositionForVisualPointer();

    void resetPointerRange();

    int leftTextMargin_;

    int rightTextMargin_;

    /// Magic number built in QT impacting text position.
    int builtInTextMargin_;

    void updateMarginSize();

private slots:
    void textWasChanged(QString newText);

    void pointerMoved(QPoint pos);

    void matchPointerToCursorPosition();
};

#endif  // ENHANCEDLINEEDIT_H
