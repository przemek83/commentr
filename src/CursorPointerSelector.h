#ifndef CURSORPOINTERSELECTOR_H
#define CURSORPOINTERSELECTOR_H

#include "CursorPointerTextEdit.h"

class CursorPointerSelector : public CursorPointerTextEdit
{
    Q_OBJECT
public:
    enum CursorDirection
    {
        CURSOR_DIRECTION_LEFT,
        CURSOR_DIRECTION_RIGHT
    };

    explicit CursorPointerSelector(CursorDirection direction,
                                   QWidget *parent = 0);

    virtual ~CursorPointerSelector();

    virtual void moveVisualPointer(int x, int y);

protected:
    virtual void paintEvent(QPaintEvent *event);

    virtual void updateSize();

    virtual QPoint calculateOffset(QMouseEvent* event);

    virtual QPoint calculateNewPosition(QPoint movePoint);

private:
    Q_DISABLE_COPY(CursorPointerSelector)

    CursorDirection diretion_;
};

#endif // CURSORPOINTERSELECTOR_H
