#include <iostream>


#include <ImfRgbaFile.h>
#include <ImfRgba.h>

#include <QtWidgets>

#include "oglWidget.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget window;
    window.resize(320, 240);
    window.show();
    window.setWindowTitle("Hello Qt5!");

    OGLWidget* oglWidget = new OGLWidget(&window);
    oglWidget->resize(300,200);
    oglWidget->show();

    return app.exec();
}