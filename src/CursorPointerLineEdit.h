#ifndef CURSORPOINTERLINEEDIT_H
#define CURSORPOINTERLINEEDIT_H

#include "CursorPointer.h"

class CursorPointerLineEdit : public CursorPointer
{
    Q_OBJECT
public:
    explicit CursorPointerLineEdit(int addLeftMargin,
                                   QWidget* parent = nullptr);

    void moveVisualPointer(int x, int y) override;

    void setScrollingNeeded(bool scrollingNeeded);

protected:
    QPoint calcMovePoint(QPoint mousePos) override;

    void positionChanged(QMouseEvent* event) override;

private:
    Q_DISABLE_COPY(CursorPointerLineEdit)

    /// Magic margin needed in some situations.
    int margin_;

    /// Flag informing that text is bigger than linenedit size, scrolling
    /// needed.
    bool scrollingNeeded_;

    /// Left margin of text. Needed in calculations.
    int addLeftMargin_;
};

#endif  // CURSORPOINTERLINEEDIT_H
