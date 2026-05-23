#pragma once

#include <QWidget>

class QLabel;

namespace UiKit {

class BannerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BannerWidget(QWidget *parent = nullptr);

private:
    QLabel *titleLabel;
};

}
