#ifndef CURSORPOINTERTEXTEDIT_H
#define CURSORPOINTERTEXTEDIT_H

#include "CursorPointer.h"

class Config;

class CursorPointerTextEdit : public CursorPointer
{
    Q_OBJECT
public:
    explicit CursorPointerTextEdit(Config& config, QWidget* parent = nullptr);
    ~CursorPointerTextEdit() override = default;

    void moveVisualPointer(int x, int y) override;

protected:
    QPoint calcMovePoint(QPoint mousePos) override;

    void positionChanged(QMouseEvent* event) override;

    virtual QPoint calculateNewPosition(QPoint movePoint);

private:
    Q_DISABLE_COPY_MOVE(CursorPointerTextEdit)

    bool canEmitPointerMoved_;

private slots:
    void allowEmitPointerMoved();
};

#endif  // CURSORPOINTERTEXTEDIT_H
