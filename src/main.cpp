#include <iostream>


#include <ImfRgbaFile.h>
#include <ImfRgba.h>

#include <QtWidgets>

#include "oglWidget.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget window;
    window.resize(320, 240);
    window.setWindowTitle("Hello Qt5!");

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);

    OGLWidget* oglWidget = new OGLWidget;
    
    mainLayout->addWidget(oglWidget);
    window.setLayout(mainLayout);
    window.show();

    return app.exec();
}