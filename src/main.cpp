#include <iostream>

#include <ImfRgbaFile.h>
#include <ImfRgba.h>

#include <QtWidgets>
#include <QtGui>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget window;
    window.resize(320, 240);
    window.show();
    window.setWindowTitle("Hello Qt5!");
    std::cout << "Hello Qt5!" << std::endl;
    return app.exec();
}