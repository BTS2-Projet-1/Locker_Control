#include <QApplication>
#include <QTextCodec>
#include "consolewindows.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setDesktopSettingsAware(false);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    ConsoleWindows w;
    w.setWindowState(w.windowState() ^ Qt::WindowFullScreen);
    w.show();
    
    return app.exec();
}
