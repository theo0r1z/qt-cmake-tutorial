#include <UiKit/bannerwidget.h>

#include <CoreKit/projectinfo.h>

#include <QLabel>
#include <QVBoxLayout>

namespace UiKit {

BannerWidget::BannerWidget(QWidget *parent)
    : QWidget(parent)
    , titleLabel(new QLabel(this))
{
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setText(CoreKit::productName() + QStringLiteral(" ") + CoreKit::versionText());

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
}

}
