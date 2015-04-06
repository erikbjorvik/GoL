#include "mainwindow.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    // create window and view it
    MainWindow window;
    window.setTitle("A Simple Graphical Window");
    window.show();

    return app.exec();
}
