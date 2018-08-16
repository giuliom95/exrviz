#include "oglWidget.hpp"

void OGLWidget::initializeGL() {
    std::cout << "Hello OpenGL!" << std::endl;
}

void OGLWidget::resizeGL(int w, int h) {
    std::cout << w << " " << h << std::endl;
}

void OGLWidget::paintGL() {
    std::cout << "Painting..." << std::endl;
}