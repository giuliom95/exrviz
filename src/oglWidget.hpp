#ifndef OGL_WIDGET_HPP
#define OGL_WIDGET_HPP

#include <iostream>
#include <vector>
#include <array>
#include <cstdint>

#include <QtWidgets>

class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
public:
    OGLWidget() : QOpenGLWidget{} {};
    OGLWidget(QWidget* parent) : QOpenGLWidget{parent} {};

    void changeImage(const std::vector<std::array<uint8_t, 4>>& inputImage, const int imageWidth, const int imageHeight);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    int imageWidth, imageHeight;
    GLuint textureId;
};

#endif