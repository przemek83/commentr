#ifndef ENHANCEDLINEEDIT_H
#define ENHANCEDLINEEDIT_H

#include <QLineEdit>

class Config;

class EnhancedLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    EnhancedLineEdit(Config& config, QWidget* parent);
    ~EnhancedLineEdit() override = default;

protected:
    bool event(QEvent* event) override;

    void changeEvent(QEvent* event) override;

private:
    Q_DISABLE_COPY_MOVE(EnhancedLineEdit)

    void updateMarginSize();

    static constexpr int leftTextMargin_{5};

    int rightTextMargin_{10};

    Config& config_;
};

#endif  // ENHANCEDLINEEDIT_H
