#include <math.h>
#include <QPainter>
#include <QTextBlock>
#include <QDebug>
#include <QGestureEvent>
#include <QScroller>
#include <QGuiApplication>
#include <QTimer>
#include <QScrollBar>
#include <QScrollEvent>
#include <QGestureRecognizer>
#include <QRegularExpression>

#include "CodeViewer.h"
#include "CursorPointerTextEdit.h"
#include "CursorPointerSelector.h"
#include "Config.h"
#include "Common.h"
#include "PanGestureRecognizer.h"

CursorEater::CursorEater(QObject* parent):
    QObject(parent)
{

}

CursorEater::~CursorEater()
{

}

bool CursorEater::eventFilter(QObject *obj, QEvent *event)
{
    if (QEvent::MouseButtonPress == event->type()
        || QEvent::MouseButtonRelease == event->type() // ||
        /*QEvent::MouseMove == event->type()*/) {
        return true;
    }

    //Standard event processing.
    return QObject::eventFilter(obj, event);
}

CodeViewer::CodeViewer(QWidget *parent) :
    QPlainTextEdit(parent),
    ignoreNextTapGesture_(false),
    cursorShownBeforeFocusLost_(false),
    selectorsShownBeforeFocusLost_(false)//,
{
    mainWindow_ = Common::getMainWindow(this);

    initVisualPointers();

    //viewport()->installEventFilter(new CursorEater(viewport()));

    QFont font;
    font.setFamily(QStringLiteral("Courier"));
    setFont(font);

    setLineWrapMode(QPlainTextEdit::NoWrap);
    setTextInteractionFlags(Qt::TextSelectableByKeyboard |
                            Qt::TextEditable);

    lineNumberArea_ = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));

    connect(this,
            SIGNAL(updateRequest(QRect,int)),
            this,
            SLOT(updateLineNumberArea(QRect,int)));

    updateLineNumberAreaWidth(0);

    connect(this,
            SIGNAL(cursorPositionChanged()),
            this,
            SLOT(cursorPosHasChanged()));

    //Hide cursors, select current line.
    cursorPosHasChanged();

    viewport()->setAttribute(Qt::WA_AcceptTouchEvents);

    grabGestures();

    setInputMethodHints(Qt::ImhNoPredictiveText);

    //In case of setting styleSheet to whole CodeEditor there is problem
    //with font size in LineNumberArea.
    horizontalScrollBar()->setStyleSheet(Common::getStyleSheet());
    verticalScrollBar()->setStyleSheet(Common::getStyleSheet());
}

CodeViewer::~CodeViewer()
{
    delete cursorPointer_;
}

void CodeViewer::grabGestures()
{
    // Do not grab tap and trap and hold as in new Qt it seems stock behaviour is working.
    // viewport()->grabGesture(Qt::TapGesture);
    // viewport()->grabGesture(Qt::TapAndHoldGesture);
    viewport()->grabGesture(Qt::PinchGesture);

    QGestureRecognizer::unregisterRecognizer(Qt::PanGesture);
    PanGestureRecognizer* panGestureRecognizer = new PanGestureRecognizer();
    Qt::GestureType gestureType = QGestureRecognizer::registerRecognizer(panGestureRecognizer);
    viewport()->grabGesture(gestureType);
}

void CodeViewer::initVisualPointers()
{
    //Cursor pointer.
    cursorPointer_ = new CursorPointerTextEdit(mainWindow_);

    connect(cursorPointer_,
            SIGNAL(pointerMoved(QPoint)),
            this,
            SLOT(pointerMoved(QPoint)));

    connect(cursorPointer_,
            SIGNAL(mouseReleased()),
            this,
            SLOT(matchPointerToCursorPosition()));

    //Cursor pointer on selection.
    cursorSelector_ =
        new CursorPointerSelector(CursorPointerSelector::CURSOR_DIRECTION_RIGHT,
                                  mainWindow_);

    connect(cursorSelector_,
            SIGNAL(pointerMoved(QPoint)),
            this,
            SLOT(pointerMoved(QPoint)));

    connect(cursorSelector_,
            SIGNAL(mouseReleased()),
            this,
            SLOT(matchPointerToCursorPosition()));

    //Anchor pointer on selection.
    anchorSelector_ =
        new CursorPointerSelector(CursorPointerSelector::CURSOR_DIRECTION_LEFT,
                                  mainWindow_);

    connect(anchorSelector_,
            SIGNAL(pointerMoved(QPoint)),
            this,
            SLOT(pointerMoved(QPoint)));

    connect(anchorSelector_,
            SIGNAL(mouseReleased()),
            this,
            SLOT(matchPointerToCursorPosition()));
}

void CodeViewer::cursorPosHasChanged()
{
    hideAllPointersIfNotDragged();

    QList<QTextEdit::ExtraSelection> extraSelections;

    QTextEdit::ExtraSelection selection;

    static QColor lineColor = QColor(Qt::yellow).lighter(160);

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);

    setExtraSelections(extraSelections);
}

bool CodeViewer::event(QEvent *event)
{
//Code from QPlainTextEdit for pan gesture.
//    if (e->type() == QEvent::Gesture)
//    {
//        QGestureEvent *ge = static_cast<QGestureEvent *>(e);
//        QPanGesture *g = static_cast<QPanGesture *>(ge->gesture(Qt::PanGesture));
//        if (g)
//        {
//            QScrollBar *hBar = horizontalScrollBar();
//            QScrollBar *vBar = verticalScrollBar();
//            if (g->state() == Qt::GestureStarted)
//                d->originalOffsetY = vBar->value();
//            QPointF offset = g->offset();
//            if (!offset.isNull())
//            {
//                if (QApplication::isRightToLeft())
//                    offset.rx() *= -1;
//                // QPlainTextEdit scrolls by lines only in vertical direction
//                QFontMetrics fm(document()->defaultFont());
//                int lineHeight = fm.height();
//                int newX = hBar->value() - g->delta().x();
//                int newY = d->originalOffsetY - offset.y()/lineHeight;
//                hBar->setValue(newX);
//                vBar->setValue(newY);
//            }
//        }
//        return true;
//    }

    if (event->type() == QEvent::Gesture)
    {
        QGestureEvent* gestureEvent = static_cast<QGestureEvent*>(event);
        QList<QGesture*> gestures = gestureEvent->gestures();

        foreach (QGesture* gesture, gestures)
        {
            switch(gesture->gestureType())
            {
                case Qt::PinchGesture:
                {
                    managePinchGesture(static_cast<QPinchGesture*>(gesture));
                    break;
                }

                case Qt::TapGesture: {
                    manageTapGesture(static_cast<QTapGesture*>(gesture));
                    break;
                }

                case Qt::TapAndHoldGesture: {
                    if (Qt::GestureFinished == gesture->state()) {
                        QTapAndHoldGesture* tapAndHoldGesture = static_cast<QTapAndHoldGesture*>(
                            gesture);
                        if (true == manageTapAndHoldGesture(tapAndHoldGesture)) {
                            ignoreNextTapGesture_ = true;
                        }
                    }

                    break;
                }

                case Qt::PanGesture:
                {
                    return QPlainTextEdit::event(event);
                }

                default:
                {
                    break;
                }
            }
        }

        event->accept();
        return true;
    }

    return QPlainTextEdit::event(event);
}

void CodeViewer::pointerMoved(QPoint pos)
{
    CursorPointerTextEdit* senderCursorPointer =
        static_cast<CursorPointerTextEdit*>(sender());

    if( senderCursorPointer == cursorPointer_ )
    {
        QTextCursor cursor = cursorForPosition(pos - positionShiftMain());
        setTextCursor(cursor);
    }
    else
    {
        int anchor = -1;
        int cursor = -1;
        if( senderCursorPointer == cursorSelector_ )
        {
            anchor = textCursor().anchor();
            cursor = cursorForPosition(pos - positionShiftMain()).position();
        }
        else
        {
            //Anchor moved.
            anchor = cursorForPosition(pos - positionShiftMain()).position();
            cursor = textCursor().position();
        }

        //Sliding pointers.
        bool updatePointer = false;
        if( anchor > cursor )
        {
            if( senderCursorPointer == cursorSelector_ )
            {
                anchor = cursor;
            }
            else
            {
                cursor = anchor;
            }
            updatePointer = true;
        }

        QTextCursor cursorToSet = textCursor();
        cursorToSet.setPosition(anchor);
        cursorToSet.setPosition(cursor, QTextCursor::KeepAnchor);
        setTextCursor(cursorToSet);

        //Sliding pointers.
        if( true == updatePointer )
        {
            //If anchor was moved than match cursor.
            //If cursor was moved by user than match anchor to it.
            if( senderCursorPointer == cursorSelector_ )
            {
                moveVisualPointer(anchorSelector_);
            }
            else
            {
                moveVisualPointer(cursorSelector_);
            }
        }
    }
}

////Temporary for desktops debug.
//void CodeViewer::mouseReleaseEvent(QMouseEvent* event)
//{
//    qDebug() << "mouseReleaseEvent" << event->pos();

//    if( true == ignoreNextTapGesture_ )
//    {
//        ignoreNextTapGesture_ = false;
//        return;
//    }

//    QTextCursor cursor =
//        cursorForPosition(event->pos());
//    setTextCursor(cursor);

//    if( 0 != document()->characterCount() )
//    {
//        setVisibleCursorPointer(true);
//    }
//}

void CodeViewer::focusOutEvent(QFocusEvent* e)
{
    cursorShownBeforeFocusLost_ = cursorPointer_->isVisible();
    selectorsShownBeforeFocusLost_ = cursorSelector_->isVisible();

    hideAllCursorPointers();

    QPlainTextEdit::focusOutEvent(e);
}

void CodeViewer::focusInEvent(QFocusEvent* e)
{
    if( true == cursorShownBeforeFocusLost_ )
    {
        setVisibleCursorPointer(cursorShownBeforeFocusLost_);
    }
    else
    {
        setVisibleSelectionPointers(selectorsShownBeforeFocusLost_);
    }

    QPlainTextEdit::focusInEvent(e);
}

void CodeViewer::managePinchGesture(QPinchGesture* gesture)
{
    switch (gesture->state())
    {
        case Qt::GestureUpdated:
        {
            const double originalFontSize = Config::getInstance().fontSize();
            const double totalScaleFactor = gesture->totalScaleFactor();
            const int expectedFontSize = round(Common::normalizeFont(originalFontSize * totalScaleFactor));
            const int currentFontSize = fontInfo().pointSizeF();
            const int change = expectedFontSize - currentFontSize;

            zoom(change);

            break;
        }

        case Qt::GestureFinished:
        {
            Config::getInstance().setFontSize(fontInfo().pointSizeF());
            break;
        }

        default:
        {
            break;
        }

    }
}

void CodeViewer::manageTapGesture(QTapGesture* gesture)
{
    if(Qt::GestureFinished == gesture->state() )
    {
        if (true == ignoreNextTapGesture_) {
            ignoreNextTapGesture_ = false;
            return;
        }

        QTextCursor cursor = cursorForPosition(gesture->position().toPoint());
        setTextCursor(cursor);

        if( 0 != document()->characterCount() )
        {
            setVisibleCursorPointer(true);
        }
    }
}

bool CodeViewer::manageTapAndHoldGesture(QTapAndHoldGesture* gesture)
{
    if (Qt::GestureFinished == gesture->state()) {
        QPoint point = mapFromGlobal(gesture->position().toPoint());
        QPoint properPoint = QPoint(point.x() - lineNumberArea_->width(), point.y());
        QTextCursor cursor = cursorForPosition(properPoint);
        cursor.select(QTextCursor::WordUnderCursor);
        setTextCursor(cursor);

        if (0 != document()->characterCount()) {
            setVisibleSelectionPointers(true);
        }

        return true;
    }

    //Needed?
    //    if( true == Config::getInstance().keyboardAfterTap() )
    //    {
    //        QGuiApplication::inputMethod()->show();
    //    }

    return false;
}

void CodeViewer::updateVisualPointersPositions()
{
    if( true == cursorPointer_->isVisible() )
    {
        moveVisualPointer(cursorPointer_);
    }

    if( true == cursorSelector_->isVisible() )
    {
        moveVisualPointer(cursorSelector_);

        if( true == anchorIsInRange() )
        {
            moveVisualPointer(anchorSelector_);
            anchorSelector_->setVisible(true);
        }
        else
        {
            anchorSelector_->setVisible(false);
        }
    }
}

/*void CodeViewer::scrollToSelection()
{
    int numberOfBlocks = blockCount();
    QTextBlock block = firstVisibleBlock();
    int firstVisibleBlockNumber = block.blockNumber();
    int currentBlockNumber = firstVisibleBlockNumber;
    int top =
        qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    int cursorBlockNumber = textCursor().blockNumber();
    int cursorDx = 0;
    int twoLinesUpDx = 0;
    int beforeLastVisibleDx = 0;
    int lastVisibleDx = 0;
    while( block.isValid() && top <= contentsRect().bottom() )
    {
        block = block.next();
        if( cursorBlockNumber == block.blockNumber() )
        {
            cursorDx = bottom;
        }
        top = bottom;
        twoLinesUpDx = beforeLastVisibleDx;
        beforeLastVisibleDx = lastVisibleDx;
        lastVisibleDx = top;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++currentBlockNumber;
    }

    int lastVisibleBlockNumber = currentBlockNumber;

    int numberOfBlocksVisible = lastVisibleBlockNumber - firstVisibleBlockNumber;

    qDebug() << __FUNCTION__ << "count" << numberOfBlocks << "first" <<
                firstVisibleBlockNumber << "last" << lastVisibleBlockNumber <<
                "cursor" << cursorBlockNumber << "visible" << numberOfBlocksVisible <<
                "cursorDx" << cursorDx << "lastVisibleDx" << lastVisibleDx << "diff" <<
                cursorDx - twoLinesUpDx;

    scroll(0, cursorDx - twoLinesUpDx);
    //ensureCursorVisible();
    ensureCursorVisible();
    qDebug() << viewport();
}*/

void CodeViewer::matchPointerToCursorPosition()
{
    CursorPointer* cursorPointer = static_cast<CursorPointer*>(sender());
    moveVisualPointer(cursorPointer);
}

void CodeViewer::moveVisualPointer(CursorPointer* cursorPointer)
{
    if( cursorPointer == anchorSelector_ )
    {
        QTextCursor cursor = textCursor();
        cursor.setPosition(cursor.anchor());
        QRect cursorRectangle = cursorRect(cursor);
        QPoint anchorPos = mapToParent(cursorRectangle.topLeft()) + positionShiftMain();
        anchorSelector_->moveVisualPointer(anchorPos.x(),
                                           anchorPos.y());
    }
    else
    {
        QRect cursorRectangle = cursorRect();
        QPoint leftTop = mapToParent(cursorRectangle.topLeft());
        QPoint newPosition = leftTop + positionShiftMain();
        cursorPointer->moveVisualPointer(newPosition.x(),
                                         newPosition.y());
    }
}

void CodeViewer::hideAllPointersIfNotDragged()
{
    if( false == cursorPointer_->dragged() &&
        false == cursorSelector_->dragged() &&
        false == anchorSelector_->dragged() )
    {
        hideAllCursorPointers();
    }
}

void CodeViewer::hideAllCursorPointers()
{
    cursorPointer_->setVisible(false);
    cursorSelector_->setVisible(false);
    anchorSelector_->setVisible(false);
}

QPoint CodeViewer::positionShiftMain()
{
    QPoint positonInMain(mapToGlobal(QPoint(0, 0)) - mainWindow_->pos());

    QRect cursorRectangle = cursorRect();

    QRect mainGeometry = mainWindow_->geometry();
    QRect mainFrameGeometry = mainWindow_->frameGeometry();

    //Difference between frame and mainwindow.
    int shiftY = mainFrameGeometry.y() - mainGeometry.y();

    //Width of Application frame (desktops).
    int frameWidth = (mainFrameGeometry.width() - mainGeometry.width())/2;

    QPoint shift = positonInMain + QPoint(lineNumberArea_->width() - frameWidth,
                                          cursorRectangle.height() + shiftY);

    return shift;
}

void CodeViewer::scrollContentsBy(int dx, int dy)
{
    hideAllPointersIfNotDragged();

    QPlainTextEdit::scrollContentsBy(dx, dy);
}

void CodeViewer::zoom(float zoomFactor)
{
    if (zoomFactor == 0)
    {
        return;
    }

    if (zoomFactor > 0)
    {
        zoomIn(zoomFactor);
    }
    else
    {
        zoomOut(abs(zoomFactor));
    }

    updateVisualPointersPositions();
}

int CodeViewer::lineNumberAreaWidth()
{
    int digits = QString::number(qMax(1, blockCount())).length();

    int space = 3 + QFontMetricsF(QGuiApplication::font()).horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void CodeViewer::updateLineNumberAreaWidth(int /*newBlockCount*/)
{
// Disable temporarily line number area on Android.
#ifdef Q_OS_ANDROID
    int lineNumbersWidth = 0;
#else
    int lineNumbersWidth = lineNumberAreaWidth();
#endif
    setViewportMargins(lineNumbersWidth, 0, 0, 0);
}

void CodeViewer::updateLineNumberArea(const QRect& rect, int dy)
{
    if (0 != dy)
    {
        //Scrolling when dy != 0.
        lineNumberArea_->scroll(0, dy);
    }
    else
    {
        //Just update.
        lineNumberArea_->update(0,
                                rect.y(),
                                lineNumberArea_->width(),
                                rect.height());
    }

    if (rect.contains(viewport()->rect()))
    {
        updateLineNumberAreaWidth(0);
    }
}

void CodeViewer::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea_->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));

    centerCursor();
    updateVisualPointersPositions();
}

void CodeViewer::lineNumberAreaPaintEvent(QPaintEvent* event)
{
// Disable temporarily line number area on Android.
#ifdef Q_OS_ANDROID
    return;
#endif

    QPainter painter(lineNumberArea_);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0,
                             top,
                             lineNumberArea_->width(),
                             fontMetrics().height(),
                             Qt::AlignRight,
                             number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void CodeViewer::paintEvent(QPaintEvent* event)
{
    QPlainTextEdit::paintEvent(event);
    const QRect rect = event->rect();
    const QFont font = currentCharFormat().font();
    int x80 = qRound(QFontMetricsF(font).averageCharWidth() * 80.0)
            + contentOffset().x()
            + document()->documentMargin();
    QPainter painter(viewport());
    painter.setPen(QPen("gray"));
    painter.drawLine(x80, rect.top(), x80, rect.bottom());
}

void CodeViewer::keyPressEvent(QKeyEvent* e)
{
    int key = e->key();
    if( Qt::Key_Return == key || Qt::Key_Enter == key )
    {
        QString line = textCursor().block().text();

        QPlainTextEdit::keyPressEvent(e);
        insertPlainText(line.left(line.indexOf(QRegularExpression("\\S"))));
//        e->accept();
//        repaint();
        return;
    }

    QPlainTextEdit::keyPressEvent(e);
}

bool CodeViewer::anchorIsInRange()
{
    QTextBlock firstBlock = firstVisibleBlock();
    QTextCursor cursor = textCursor();
    cursor.setPosition(cursor.anchor());
    if( cursor.blockNumber() < firstBlock.blockNumber() )
    {
        return false;
    }

    return true;
}

void CodeViewer::setVisibleCursorPointer(bool visible)
{
    if( true == visible )
    {
        moveVisualPointer(cursorPointer_);
    }

    cursorPointer_->setVisible(visible);

    cursorSelector_->setVisible(false);
    anchorSelector_->setVisible(false);
}

void CodeViewer::setVisibleSelectionPointers(bool visible)
{
    cursorPointer_->setVisible(false);

    if( true == visible )
    {
        moveVisualPointer(anchorSelector_);
        moveVisualPointer(cursorSelector_);
    }

    cursorSelector_->setVisible(visible);
    anchorSelector_->setVisible(visible);
}
