#ifndef ENHANCEDLINEEDIT_H
#define ENHANCEDLINEEDIT_H

#include <QLineEdit>

class CursorPointerLineEdit;
class QGestureEvent;

class EnhancedLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit EnhancedLineEdit(QWidget *parent = 0);

    virtual ~EnhancedLineEdit();

protected:
    virtual void mouseReleaseEvent(QMouseEvent* e);

    virtual void focusOutEvent(QFocusEvent* event);

    virtual void resizeEvent(QResizeEvent* event);

    virtual bool event(QEvent* event);

    virtual void changeEvent(QEvent* event);

private:
    Q_DISABLE_COPY(EnhancedLineEdit)

    ///Visual cursor.
    CursorPointerLineEdit* cursorPointer_;

    QWidget* mainWindow_;

    QWidget* getMainWindow();

    QPoint getPositionForVisualPointer();

    void resetPointerRange();

    int leftTextMargin_;

    int rightTextMargin_;

    ///Magic number built in QT impacting text position.
    int builtInTextMargin_;

    void updateMarginSize();

private slots:
    void textWasChanged(QString newText);

    void pointerMoved(QPoint pos);

    void matchPointerToCursorPosition();
};

#endif // ENHANCEDLINEEDIT_H
