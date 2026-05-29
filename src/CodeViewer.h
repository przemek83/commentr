#ifndef CODEVIEWER_H
#define CODEVIEWER_H

#include <QObject>
#include <QPlainTextEdit>

#include "CursorPointerSelector.h"
#include "CursorPointerTextEdit.h"

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class QGestureEvent;
class QPinchGesture;
class QTapGesture;
class QTapAndHoldGesture;
class CursorPointer;
class Config;

class CodeViewer : public QPlainTextEdit
{
    Q_OBJECT
public:
    CodeViewer(Config& config, QWidget* parent);

    ~CodeViewer() override = default;

    void zoom(int zoomFactor);

    void updateVisualPointersPositions();

protected:
    void resizeEvent(QResizeEvent* e) override;

    void paintEvent(QPaintEvent* e) override;

    void keyPressEvent(QKeyEvent* e) override;

    bool event(QEvent* e) override;

    void scrollContentsBy(int dx, int dy) override;

    void focusOutEvent(QFocusEvent* e) override;

    void focusInEvent(QFocusEvent* e) override;

private:
    Q_DISABLE_COPY_MOVE(CodeViewer)

    int lineNumberAreaWidth() const;

    void lineNumberAreaPaintEvent(const QPaintEvent* event);

    class LineNumberArea : public QWidget
    {
    public:
        explicit LineNumberArea(CodeViewer* editor)
            : QWidget(editor), codeEditor_{editor}
        {
        }

        QSize sizeHint() const override
        {
            return QSize{codeEditor_->lineNumberAreaWidth(), 0};
        }

    protected:
        void paintEvent(QPaintEvent* event) override
        {
            codeEditor_->lineNumberAreaPaintEvent(event);
        }

    private:
        CodeViewer* codeEditor_;
    };

    LineNumberArea* lineNumberArea_;

    friend class LineNumberArea;

    QWidget* mainWindow_;

    void managePinchGesture(const QPinchGesture* gesture);

    void manageTapGesture(const QTapGesture* gesture);

    void manageTapAndHoldGesture(const QTapAndHoldGesture* gesture);

    QPoint positionShiftMain() const;

    bool ignoreNextTapGesture_{false};

    void initVisualPointers() const;

    void moveVisualPointer(CursorPointer* cursorPointer);

    void hideAllCursorPointers();

    void setVisibleCursorPointer(bool visible);

    void setVisibleSelectionPointers(bool visible);

    bool cursorShownBeforeFocusLost_{false};

    bool selectorsShownBeforeFocusLost_{false};

    bool anchorIsInRange() const;

    void hideAllPointersIfNotDragged();

    void grabGestures() const;

    Config& config_;

    CursorPointerTextEdit cursorPointer_;

    CursorPointerSelector cursorSelector_;

    CursorPointerSelector anchorSelector_;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);

    void updateLineNumberArea(const QRect& rect, int dy);

    void matchPointerToCursorPosition();

    void pointerMoved(QPoint pos);

    void cursorPosHasChanged();
};

#endif  // CODEVIEWER_H
