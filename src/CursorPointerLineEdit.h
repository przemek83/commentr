#ifndef CURSORPOINTERLINEEDIT_H
#define CURSORPOINTERLINEEDIT_H

#include "CursorPointer.h"

class CursorPointerLineEdit : public CursorPointer
{
    Q_OBJECT
public:
    explicit CursorPointerLineEdit(int addLeftMargin, QWidget *parent = 0);

    virtual ~CursorPointerLineEdit();

    virtual void moveVisualPointer(int x, int y);

    void setScrollingNeeded(bool scrollingNeeded);

protected:
    virtual QPoint calcMovePoint(QPoint mousePos);

    virtual void positionChanged(QMouseEvent* event);

private:
    Q_DISABLE_COPY(CursorPointerLineEdit)

    ///Magic margin needed in some situations.
    int margin_;

    ///Flag informing that text is bigger than linenedit size, scrolling needed.
    bool scrollingNeeded_;

    ///Left margin of text. Needed in calculations.
    int addLeftMargin_;
};

#endif // CURSORPOINTERLINEEDIT_H
