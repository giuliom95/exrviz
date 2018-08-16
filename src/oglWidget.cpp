#include "oglWidget.hpp"

void OGLWidget::initializeGL() {
    std::cout << "Hello OpenGL!" << std::endl;
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
}

void OGLWidget::resizeGL(int w, int h) {
    std::cout << w << " " << h << std::endl;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, -1, +1);
    glMatrixMode( GL_MODELVIEW ); 
    glLoadIdentity();
}

void OGLWidget::paintGL() {
    std::cout << "Painting..." << std::endl;
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin( GL_QUADS ); 
        glVertex2f(   0,   0 );
        glVertex2f( 200,   0 );
        glVertex2f( 200, 200 );
        glVertex2f(   0, 200 );
    glEnd();
}