#ifndef OGL_WIDGET_HPP
#define OGL_WIDGET_HPP

#include <iostream>

#include <QtWidgets>

class OGLWidget : public QOpenGLWidget {
public:
    OGLWidget() : QOpenGLWidget{} {};
    OGLWidget(QWidget* parent) : QOpenGLWidget{parent} {};

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
};

#endif