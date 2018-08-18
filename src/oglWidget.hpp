#ifndef OGL_WIDGET_HPP
#define OGL_WIDGET_HPP

#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <cstdint>

#include <QtWidgets>

class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
public:
	OGLWidget();

	void changeImage(const std::vector<std::array<uint8_t, 4>>& inputImage, const int imageWidth, const int imageHeight);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void wheelEvent(QWheelEvent *event);

private:
	int		widgetWidth,	widgetHeight;
	int		imageWidth,		imageHeight;
	float	cameraPanX,		cameraPanY;
	float	lastMouseX,		lastMouseY;

	float zoomFactor;
	float widgetAspectRatio;

	GLuint textureId;

	bool mousePressed;
};

#endif