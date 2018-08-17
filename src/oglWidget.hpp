#ifndef OGL_WIDGET_HPP
#define OGL_WIDGET_HPP

#include <iostream>
#include <vector>
#include <array>

#include <QtWidgets>

class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
public:
    OGLWidget() : QOpenGLWidget{} {};
    OGLWidget(QWidget* parent) : QOpenGLWidget{parent} {};

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    GLuint textureId;
};

#endif