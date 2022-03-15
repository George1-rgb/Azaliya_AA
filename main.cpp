#include "Engine/widget.h"
#include "Engine/Global.h"
#include <QApplication>
#include "Engine/MainWindow.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QApplication a(argc, argv);
    QSurfaceFormat format;
    format.setSamples(16);
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow window;

    window.show();
    return a.exec();
}
