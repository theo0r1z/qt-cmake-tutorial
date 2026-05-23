#include <UiKit/bannerwidget.h>

#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle(QStringLiteral("Designer Tool"));
    window.setCentralWidget(new UiKit::BannerWidget(&window));
    window.resize(720, 480);
    window.show();

    return app.exec();
}
