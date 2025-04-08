#ifndef CURSORPOINTERTEXTEDIT_H
#define CURSORPOINTERTEXTEDIT_H

#include "CursorPointer.h"

class CursorPointerTextEdit : public CursorPointer
{
    Q_OBJECT
public:
    explicit CursorPointerTextEdit(QWidget* parent = nullptr);

    void moveVisualPointer(int x, int y) override;

protected:
    QPoint calcMovePoint(QPoint mousePos) override;

    void positionChanged(QMouseEvent* event) override;

    virtual QPoint calculateNewPosition(QPoint movePoint);

private:
    Q_DISABLE_COPY(CursorPointerTextEdit)

    bool canEmitPointerMoved_;

private slots:
    void allowEmitPointerMoved();
};

#endif  // CURSORPOINTERTEXTEDIT_H
