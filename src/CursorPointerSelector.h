#ifndef CURSORPOINTERSELECTOR_H
#define CURSORPOINTERSELECTOR_H

#include "CursorPointerTextEdit.h"

class CursorPointerSelector : public CursorPointerTextEdit
{
    Q_OBJECT
public:
    enum class CursorDirection
    {
        LEFT,
        RIGHT
    };

    explicit CursorPointerSelector(CursorDirection direction,
                                   QWidget* parent = nullptr);
    ~CursorPointerSelector() override = default;

    void moveVisualPointer(int x, int y) override;

protected:
    void paintEvent(QPaintEvent* event) override;

    QPoint calculateOffset(QMouseEvent* event) override;

    QPoint calculateNewPosition(QPoint movePoint) override;

private:
    Q_DISABLE_COPY_MOVE(CursorPointerSelector)

    CursorDirection diretion_;
};

#endif  // CURSORPOINTERSELECTOR_H
