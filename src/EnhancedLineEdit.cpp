#include "EnhancedLineEdit.h"

#include <QGestureEvent>
#include <QGuiApplication>

#include "Config.h"
#include "CursorPointerLineEdit.h"

EnhancedLineEdit::EnhancedLineEdit(QWidget* parent)
    : QLineEdit(parent),
      leftTextMargin_(5),
      rightTextMargin_(10),
      builtInTextMargin_(4)
{
    cursorPointer_ = new CursorPointerLineEdit(leftTextMargin_ + 6, parent);

    mainWindow_ = getMainWindow();

    connect(this, SIGNAL(textChanged(QString)), this,
            SLOT(textWasChanged(QString)));

    connect(cursorPointer_, SIGNAL(pointerMoved(QPoint)), this,
            SLOT(pointerMoved(QPoint)));

    connect(cursorPointer_, SIGNAL(mouseReleased()), this,
            SLOT(matchPointerToCursorPosition()));

    updateMarginSize();

    QSizePolicy sizePolicy;
    sizePolicy.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
    sizePolicy.setHorizontalStretch(255);
    setSizePolicy(sizePolicy);
}

bool EnhancedLineEdit::event(QEvent* event)
{
    if (event->type() == QEvent::MouseMove)
    {
        event->ignore();
        return true;
    }

    return QLineEdit::event(event);
}

void EnhancedLineEdit::mouseReleaseEvent(QMouseEvent* e)
{
    if (!text().isEmpty() && !isReadOnly())
    {
        cursorPointer_->show();
        cursorPointer_->raise();
    }

    matchPointerToCursorPosition();

    QLineEdit::mouseReleaseEvent(e);

    // If toolbar is shown on main screen wrong results generated.
    // Problem wil be solved when file browser will be rebuild on show.
    resetPointerRange();
}

QWidget* EnhancedLineEdit::getMainWindow()
{
    QWidget* widget = this;
    while (widget->parentWidget() != nullptr)
        widget = widget->parentWidget();

    return widget;
}

void EnhancedLineEdit::focusOutEvent(QFocusEvent* event)
{
    cursorPointer_->hide();
    QLineEdit::focusOutEvent(event);
}

void EnhancedLineEdit::resizeEvent(QResizeEvent* event)
{
    cursorPointer_->hide();
    QLineEdit::resizeEvent(event);
    resetPointerRange();
}

void EnhancedLineEdit::resetPointerRange()
{
    QPoint newPosition = getPositionForVisualPointer();
    int textWidth =
        QFontMetricsF(QGuiApplication::font()).horizontalAdvance(text());
    int textPlusMarginsWidth =
        textWidth + leftTextMargin_ + rightTextMargin_ + 2 * builtInTextMargin_;
    bool scrollingNeeded = (textPlusMarginsWidth > width());

    // Create rectangle for lineedit input. use  leftTextMargin_  + 6 px
    // because of text margins.
    QRect rect(newPosition.x(), newPosition.y(),
               qMin(textWidth + leftTextMargin_ + 6, width()), 0);

    cursorPointer_->setRange(rect);
    cursorPointer_->setScrollingNeeded(scrollingNeeded);
}

QPoint EnhancedLineEdit::getPositionForVisualPointer() const
{
    // Position of lineedit in application.
    QPoint lineEditPosInMain(mapToGlobal(QPoint(0, 0)) - mainWindow_->pos());

    QRect mainGeometry = mainWindow_->geometry();
    QRect mainFrameGeometry = mainWindow_->frameGeometry();

    // Difference between frame and mainwindow.
    int shiftY = mainFrameGeometry.y() - mainGeometry.y();

    // Width of Application frame (desktops).
    int frameWidth = (mainFrameGeometry.width() - mainGeometry.width()) / 2;

    // Calculate new position of visual pointer. Sub 1 px to match cursor pos.
    int newX = lineEditPosInMain.x() - frameWidth - 1;
    int newY = lineEditPosInMain.y() + shiftY + height();

    return QPoint(newX, newY);
}

void EnhancedLineEdit::textWasChanged(QString /*newText*/)
{
    cursorPointer_->hide();
    resetPointerRange();
}

void EnhancedLineEdit::pointerMoved(QPoint pos)
{
    setCursorPosition(cursorPositionAt(pos));
}

void EnhancedLineEdit::matchPointerToCursorPosition()
{
    QPoint newPosition = getPositionForVisualPointer();
    cursorPointer_->moveVisualPointer(newPosition.x() + cursorRect().right(),
                                      newPosition.y());
}

void EnhancedLineEdit::changeEvent(QEvent* event)
{
    QLineEdit::changeEvent(event);

    // If toolbar is shown on main screen wrong results generated.
    if (QEvent::StyleChange == event->type())
    {
        updateMarginSize();
        resetPointerRange();
    }
}

void EnhancedLineEdit::updateMarginSize()
{
    rightTextMargin_ = Config::getInstance().uiSize() / 2;

    // Add margins to enable scrolling.
    setTextMargins(leftTextMargin_, 0, rightTextMargin_, 0);
}
