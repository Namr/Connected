#include "connected.h"
#include <QApplication>
#include <QWindow>
#define LOOKINGGLASS

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    Connected w;
    w.show();
#ifdef LOOKINGGLASS
    w.windowHandle()->setScreen(qApp->screens()[1]);
    w.windowHandle()->setFlag(Qt::FramelessWindowHint);
    w.setAttribute(Qt::WA_NoSystemBackground, true);
    w.showFullScreen();
#endif
    return a.exec();
}
