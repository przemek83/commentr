#ifndef CODEVIEWER_H
#define CODEVIEWER_H

#include <QObject>
#include <QPlainTextEdit>

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

protected:
    void resizeEvent(QResizeEvent* e) override;

    void paintEvent(QPaintEvent* e) override;

    void keyPressEvent(QKeyEvent* e) override;

    bool event(QEvent* e) override;

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

    QWidget* mainWindow_;

    void managePinchGesture(const QPinchGesture* gesture);

    int calculateZoomFactor(const QPinchGesture* gesture) const;

    void manageTapGesture(const QTapGesture* gesture);

    void manageTapAndHoldGesture(const QTapAndHoldGesture* gesture);

    QPoint positionShiftMain() const;

    bool ignoreNextTapGesture_{false};

    void grabGestures() const;

    Config& config_;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);

    void updateLineNumberArea(const QRect& rect, int dy);

    void cursorPosHasChanged();
};

#endif  // CODEVIEWER_H
