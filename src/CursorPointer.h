#ifndef CURSORPOINTER_H
#define CURSORPOINTER_H

#include <QWidget>

class CursorPointer : public QWidget
{
    Q_OBJECT
public:
    explicit CursorPointer(QWidget *parent = 0);

    virtual ~CursorPointer();

    virtual void moveVisualPointer(int x, int y) = 0;

    void setRange(QRect range);

    bool dragged() const;

    void showCursor();

protected:
    virtual void paintEvent(QPaintEvent *event);

    virtual void mousePressEvent(QMouseEvent* event);

    virtual void mouseMoveEvent(QMouseEvent* event);

    virtual void mouseReleaseEvent(QMouseEvent* event);

    virtual void changeEvent(QEvent* event);

    virtual QPoint calcMovePoint(QPoint mousePos) = 0;

    virtual void positionChanged(QMouseEvent* event) = 0;

    ///Middle of pointer saved on click.
    QPoint offset_;

    ///Range of text in lineedit where pointer can move.
    QRect range_;

    ///Size of pointer.
    int size_;

    virtual void updateSize();

    virtual QPoint calculateOffset(QMouseEvent* event);

private:
    Q_DISABLE_COPY(CursorPointer)

    bool dragged_;

signals:
    void pointerMoved(QPoint pos);

    void mouseReleased();
};

#endif // CURSORPOINTER_H
