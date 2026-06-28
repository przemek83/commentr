#include "EnhancedLineEdit.h"

#include <QEvent>

#include "Config.h"

EnhancedLineEdit::EnhancedLineEdit(Config& config, QWidget* parent)
    : QLineEdit(parent),
      config_(config)
{
    updateMarginSize();

    QSizePolicy sizePolicy;
    sizePolicy.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
    constexpr int maxHorizontalStretch{255};
    sizePolicy.setHorizontalStretch(maxHorizontalStretch);
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

void EnhancedLineEdit::changeEvent(QEvent* event)
{
    QLineEdit::changeEvent(event);

    // Wrong results are generated if toolbar is shown on main screen.
    if (QEvent::StyleChange == event->type())
        updateMarginSize();
}

void EnhancedLineEdit::updateMarginSize()
{
    static constexpr int halfDivisor{2};
    rightTextMargin_ = static_cast<int>(config_.uiSize() / halfDivisor);

    // Add margins to enable scrolling.
    setTextMargins(leftTextMargin_, 0, rightTextMargin_, 0);
}
