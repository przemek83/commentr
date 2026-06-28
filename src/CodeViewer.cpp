#include "CodeViewer.h"

#include <QGestureEvent>
#include <QGestureRecognizer>
#include <QGuiApplication>
#include <QPainter>
#include <QRegularExpression>
#include <QScrollBar>
#include <QScrollEvent>
#include <QScroller>
#include <QTextBlock>
#include <QTimer>

#include "Common.h"
#include "Config.h"
#include "PanGestureRecognizer.h"

CodeViewer::CodeViewer(Config& config, QWidget* parent)
    : QPlainTextEdit(parent),
      lineNumberArea_(new LineNumberArea(this)),
      mainWindow_(Common::getMainWindow(this)),
      config_{config}
{
    QFont font;
    font.setFamily(QStringLiteral("Courier"));
    setFont(font);

    setLineWrapMode(QPlainTextEdit::NoWrap);
    setTextInteractionFlags(Qt::TextSelectableByKeyboard | Qt::TextEditable);

    connect(this, &CodeViewer::blockCountChanged, this,
            &CodeViewer::updateLineNumberAreaWidth);

    connect(this, &CodeViewer::updateRequest, this,
            &CodeViewer::updateLineNumberArea);

    updateLineNumberAreaWidth(0);

    connect(this, &CodeViewer::cursorPositionChanged, this,
            &CodeViewer::cursorPosHasChanged);

    // Hide cursors, select current line.
    cursorPosHasChanged();

    viewport()->setAttribute(Qt::WA_AcceptTouchEvents);

    grabGestures();

    setInputMethodHints(Qt::ImhNoPredictiveText);

    // In case of setting styleSheet to whole CodeEditor there is problem
    // with font size in LineNumberArea.
    horizontalScrollBar()->setStyleSheet(Common::getStyleSheet());
    verticalScrollBar()->setStyleSheet(Common::getStyleSheet());
}

void CodeViewer::grabGestures() const
{
    // Do not grab tap and tap and hold as in new Qt it seems stock behaviour
    // is working.
    viewport()->grabGesture(Qt::PinchGesture);

    QGestureRecognizer::unregisterRecognizer(Qt::PanGesture);
    auto* panGestureRecognizer{new PanGestureRecognizer()};
    Qt::GestureType gestureType =
        QGestureRecognizer::registerRecognizer(panGestureRecognizer);
    viewport()->grabGesture(gestureType);
}

void CodeViewer::cursorPosHasChanged()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    QTextEdit::ExtraSelection selection;

    constexpr int currentLineLightness{160};
    const QColor lineColor{QColor(Qt::yellow).lighter(currentLineLightness)};

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);

    setExtraSelections(extraSelections);
}

bool CodeViewer::event(QEvent* e)
{
    if (e->type() != QEvent::Gesture)
        return QPlainTextEdit::event(e);

    const auto* gestureEvent{dynamic_cast<QGestureEvent*>(e)};
    QList<QGesture*> gestures{gestureEvent->gestures()};

    for (QGesture* gesture : gestures)
    {
        switch (gesture->gestureType())
        {
            case Qt::PinchGesture:
            {
                managePinchGesture(dynamic_cast<QPinchGesture*>(gesture));
                break;
            }

            case Qt::TapGesture:
            {
                manageTapGesture(dynamic_cast<QTapGesture*>(gesture));
                break;
            }

            case Qt::TapAndHoldGesture:
            {
                manageTapAndHoldGesture(
                    dynamic_cast<QTapAndHoldGesture*>(gesture));

                break;
            }

            case Qt::PanGesture:
            {
                return QPlainTextEdit::event(e);
            }

            default:
            {
                break;
            }
        }
    }

    e->accept();
    return true;
}

void CodeViewer::managePinchGesture(const QPinchGesture* gesture)
{
    switch (gesture->state())
    {
        case Qt::GestureUpdated:
        {
            const int zoomFactor{calculateZoomFactor(gesture)};
            zoom(zoomFactor);
            break;
        }

        case Qt::GestureFinished:
        {
            config_.setFontSize(static_cast<float>(fontInfo().pointSizeF()));
            break;
        }

        default:
        {
            break;
        }
    }
}

int CodeViewer::calculateZoomFactor(const QPinchGesture* gesture) const
{
    const float originalFontSize{config_.fontSize()};
    const auto totalScaleFactor{
        static_cast<float>(gesture->totalScaleFactor())};
    const double expectedFontSize{
        std::round(Common::normalizeFont(originalFontSize * totalScaleFactor))};
    const double currentFontSize{fontInfo().pointSizeF()};
    const auto change{
        static_cast<int>(std::round(expectedFontSize - currentFontSize))};
    return change;
}

void CodeViewer::manageTapGesture(const QTapGesture* gesture)
{
    if (Qt::GestureFinished != gesture->state())
        return;

    if (ignoreNextTapGesture_)
    {
        ignoreNextTapGesture_ = false;
        return;
    }

    QTextCursor cursor{cursorForPosition(gesture->position().toPoint())};
    setTextCursor(cursor);
}

void CodeViewer::manageTapAndHoldGesture(const QTapAndHoldGesture* gesture)
{
    if (Qt::GestureFinished != gesture->state())
        return;

    QPoint point{mapFromGlobal(gesture->position().toPoint())};
    QPoint properPoint(point.x() - lineNumberArea_->width(), point.y());
    QTextCursor cursor{cursorForPosition(properPoint)};
    cursor.select(QTextCursor::WordUnderCursor);
    setTextCursor(cursor);

    ignoreNextTapGesture_ = true;
}

QPoint CodeViewer::positionShiftMain() const
{
    QPoint positonInMain(mapToGlobal(QPoint(0, 0)) - mainWindow_->pos());

    QRect cursorRectangle = cursorRect();

    QRect mainGeometry = mainWindow_->geometry();
    QRect mainFrameGeometry = mainWindow_->frameGeometry();

    // Difference between frame and mainwindow.
    int shiftY = mainFrameGeometry.y() - mainGeometry.y();

    // Width of Application frame (desktops).
    int frameWidth = (mainFrameGeometry.width() - mainGeometry.width()) / 2;

    QPoint shift = positonInMain + QPoint(lineNumberArea_->width() - frameWidth,
                                          cursorRectangle.height() + shiftY);

    return shift;
}

void CodeViewer::zoom(int zoomFactor)
{
    if (zoomFactor == 0)
        return;

    if (zoomFactor > 0)
        zoomIn(zoomFactor);
    else
        zoomOut(std::abs(zoomFactor));
}

int CodeViewer::lineNumberAreaWidth() const
{
    const auto digits =
        static_cast<int>(QString::number(::qMax(1, blockCount())).length());
    constexpr int lineNumberPadding{3};
    const double digitWidth{QFontMetricsF(QGuiApplication::font())
                                .horizontalAdvance(QLatin1Char('9'))};
    return lineNumberPadding + static_cast<int>(digitWidth * digits);
}

void CodeViewer::updateLineNumberAreaWidth([[maybe_unused]] int newBlockCount)
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
    if (dy != 0)
        lineNumberArea_->scroll(0, dy);
    else
        lineNumberArea_->update(0, rect.y(), lineNumberArea_->width(),
                                rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeViewer::resizeEvent(QResizeEvent* e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea_->setGeometry(
        QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));

    centerCursor();
}

void CodeViewer::lineNumberAreaPaintEvent(const QPaintEvent* event)
{
// Disable temporarily line number area on Android.
#ifdef Q_OS_ANDROID
    return;
#endif

    QPainter painter(lineNumberArea_);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = ::qRound(
        blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + ::qRound(blockBoundingRect(block).height());

    while (block.isValid() && (top <= event->rect().bottom()))
    {
        if (block.isVisible() && (bottom >= event->rect().top()))
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea_->width(),
                             fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + ::qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void CodeViewer::paintEvent(QPaintEvent* e)
{
    QPlainTextEdit::paintEvent(e);
    const QRect rect{e->rect()};
    const QFont font{currentCharFormat().font()};
    constexpr double rightGuideColumn{80.0};
    const double sizeOf80Chars{QFontMetricsF(font).averageCharWidth() *
                               rightGuideColumn};
    int x80{::qRound(sizeOf80Chars + contentOffset().x() +
                     document()->documentMargin())};
    QPainter painter(viewport());
    painter.setPen(QPen("gray"));
    painter.drawLine(x80, rect.top(), x80, rect.bottom());
}

void CodeViewer::keyPressEvent(QKeyEvent* e)
{
    if (const int key{e->key()};
        (Qt::Key_Return == key) || (Qt::Key_Enter == key))
    {
        QString line{textCursor().block().text()};

        QPlainTextEdit::keyPressEvent(e);
        insertPlainText(
            line.left(line.indexOf(QRegularExpression(QStringLiteral("\\S")))));
        return;
    }

    QPlainTextEdit::keyPressEvent(e);
}
