#include <QDebug>
#include <QKeyEvent>
#include <QWidget>

#include "BackButtonHandler.h"

bool BackButtonHandler::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::KeyRelease)
    {
        const auto* keyEvent{dynamic_cast<QKeyEvent*>(event)};
        if ((keyEvent != nullptr) && (keyEvent->key() == Qt::Key_Back))
        {
            auto* widget{dynamic_cast<QWidget*>(watched)};
            if (widget != nullptr)
            {
                widget->clearFocus();
                return true;
            }
        }
    }

    return QObject::eventFilter(watched, event);
}
