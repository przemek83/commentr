#ifndef CODEVIEWER_H
#define CODEVIEWER_H

#include <QPlainTextEdit>
#include <QObject>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class QGestureEvent;
class CursorPointerTextEdit;
class QPinchGesture;
class QTapGesture;
class QTapAndHoldGesture;
class CursorPointerSelector;
class CursorPointer;

class CursorEater : public QObject
{
Q_OBJECT
public:
    explicit CursorEater(QObject* parent);

    virtual ~CursorEater();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Q_DISABLE_COPY(CursorEater)
};

class CodeViewer : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit CodeViewer(QWidget *parent = 0);

    virtual ~CodeViewer();

    void zoom(float zoomFactor);

    void updateVisualPointersPositions();

protected:
//    virtual void mouseReleaseEvent(QMouseEvent* event);

    virtual void resizeEvent(QResizeEvent* event);

    virtual void paintEvent(QPaintEvent* event);

    virtual void keyPressEvent(QKeyEvent* e);

    virtual bool event(QEvent* event);

    virtual void scrollContentsBy(int dx, int dy);

    virtual void focusOutEvent(QFocusEvent* e);

    virtual void focusInEvent(QFocusEvent* e);

private:
    Q_DISABLE_COPY(CodeViewer)

    int lineNumberAreaWidth();

    void lineNumberAreaPaintEvent(QPaintEvent *event);

    // Visual cursor.
    CursorPointerTextEdit* cursorPointer_;

    CursorPointerSelector* cursorSelector_;

    CursorPointerSelector* anchorSelector_;

    class LineNumberArea : public QWidget
    {
    public:
        LineNumberArea(CodeViewer *editor) : QWidget(editor)
        {
            codeEditor_ = editor;
        }

        QSize sizeHint() const
        {
            return QSize(codeEditor_->lineNumberAreaWidth(), 0);
        }

    protected:
        void paintEvent(QPaintEvent *event)
        {
            codeEditor_->lineNumberAreaPaintEvent(event);
        }

    private:
        CodeViewer* codeEditor_;
    };

    LineNumberArea* lineNumberArea_;

    friend class LineNumberArea;

    QWidget* mainWindow_;

    void managePinchGesture(QPinchGesture* gesture);

    void manageTapGesture(QTapGesture* gesture);

    bool manageTapAndHoldGesture(QTapAndHoldGesture* gesture);

    QPoint positionShiftMain();

    bool ignoreNextTapGesture_;

    void initVisualPointers();

    void moveVisualPointer(CursorPointer* cursorPointer);

    void hideAllCursorPointers();

    void setVisibleCursorPointer(bool visible);

    void setVisibleSelectionPointers(bool visible);

    bool cursorShownBeforeFocusLost_;

    bool selectorsShownBeforeFocusLost_;

    /**
     * @brief Check if anchor is displayed on screen.
     * @return Anchor is on visible part of screen.
     */
    bool anchorIsInRange();

    /**
     * @brief Hide visual pointers when cursor was moved but no dragging used.
     */
    void hideAllPointersIfNotDragged();

    void grabGestures();

private slots:
    /**
     * @brief Resize area where editor will be shown.
     * @param newBlockCount New number of blocks in editor.
     */
    void updateLineNumberAreaWidth(int newBlockCount);

    /**
     * @brief Update line number area rectangle.
     * @param rect Rectangle to update.
     * @param dy Number of pixels scrolled.
     */
    void updateLineNumberArea(const QRect& rect, int dy);

    void matchPointerToCursorPosition();

    void pointerMoved(QPoint pos);

    /**
     * @brief React when cursor position in editor was changed.
     */
    void cursorPosHasChanged();

//    void virtualKeyboardShown();
};

#endif // CODEVIEWER_H
