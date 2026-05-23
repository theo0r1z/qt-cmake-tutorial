#include "colorpanel.h"

#include <QHBoxLayout>
#include <QLabel>

ColorPanel::ColorPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel(QStringLiteral("Nested plugin-like module"), this));
}
