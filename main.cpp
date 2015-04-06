#include "mainwindow.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    // create window and view it
    MainWindow window;
    window.setTitle("Game of Life Erik Bjørvik");
    window.show();

    return app.exec();
}
