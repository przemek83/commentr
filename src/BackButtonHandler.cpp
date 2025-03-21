#include <QDebug>
#include <QKeyEvent>
#include <QWidget>

#include "BackButtonHandler.h"

BackButtonHandler::BackButtonHandler(QObject* parent) : QObject(parent) {}

BackButtonHandler::~BackButtonHandler() {}

bool BackButtonHandler::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyRelease)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent != nullptr && Qt::Key_Back == keyEvent->key())
        {
            QWidget* widget = static_cast<QWidget*>(obj);
            if (widget != nullptr)
            {
                widget->clearFocus();
                return true;
            }
        }
    }

    // standard event processing
    return QObject::eventFilter(obj, event);
}
