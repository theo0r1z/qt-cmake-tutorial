#include <CoreKit/projectinfo.h>

#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QLabel label(CoreKit::productName() + QStringLiteral(" image viewer"));
    label.setAlignment(Qt::AlignCenter);
    label.resize(520, 320);
    label.show();

    return app.exec();
}
