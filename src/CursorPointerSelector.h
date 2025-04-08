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
                                   QWidget* parent = nullptr);

    void moveVisualPointer(int x, int y) override;

protected:
    void paintEvent(QPaintEvent* event) override;

    void updateSize() override;

    QPoint calculateOffset(QMouseEvent* event) override;

    QPoint calculateNewPosition(QPoint movePoint) override;

private:
    Q_DISABLE_COPY(CursorPointerSelector)

    CursorDirection diretion_;
};

#endif  // CURSORPOINTERSELECTOR_H
