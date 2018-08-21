#ifndef OGL_WIDGET_HPP
#define OGL_WIDGET_HPP

#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <cstdint>
#include <string>
#include <iomanip>
#include <sstream>

#include <QtWidgets>

#include <ImfRgbaFile.h>
#include <ImfRgba.h>

class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT
public:
	OGLWidget(QLabel& pixelInfoLabel, QPushButton& zoomButton);

	void changeImage(	const std::vector<Imf::Rgba>& inputHdrImage,
						const int imageWidth, const int imageHeight);

	void setZoom(float zoomFactor, float centerX, float centerY);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void wheelEvent(QWheelEvent *event);

private:
	std::vector<Imf::Rgba> hdrImage;

	int		widgetWidth,	widgetHeight;
	int		imageWidth,		imageHeight;
	float	cameraPanX,		cameraPanY;
	float	lastMouseX,		lastMouseY;

	float zoomFactor;
	float widgetAspectRatio;

	GLuint textureId;

	bool mousePressed;

	QLabel& pixelInfoLabel;
	QPushButton& zoomButton;
};

#endif