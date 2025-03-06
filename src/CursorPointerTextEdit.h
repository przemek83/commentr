#ifndef CURSORPOINTERTEXTEDIT_H
#define CURSORPOINTERTEXTEDIT_H

#include "CursorPointer.h"

class CursorPointerTextEdit : public CursorPointer
{
    Q_OBJECT
public:
    explicit CursorPointerTextEdit(QWidget *parent = 0);

    virtual ~CursorPointerTextEdit();

    virtual void moveVisualPointer(int x, int y);

protected:
    virtual QPoint calcMovePoint(QPoint mousePos);

    virtual void positionChanged(QMouseEvent* event);

    virtual QPoint calculateNewPosition(QPoint movePoint);

private:
    Q_DISABLE_COPY(CursorPointerTextEdit)

    bool canEmitPointerMoved_;

private slots:
    void allowEmitPointerMoved();
};

#endif // CURSORPOINTERTEXTEDIT_H
