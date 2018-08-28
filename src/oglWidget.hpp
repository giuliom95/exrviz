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
#include <thread>

#include <QtWidgets>

#include <ImfRgbaFile.h>
#include <ImfRgba.h>

class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT
public:
	OGLWidget(QLabel& pixelInfoLabel, QPushButton& zoomButton);

	void changeImage(	const std::vector<Imf::Rgba>& inputHdrImage,
						const int imageWidth, const int imageHeight);

	void changeExposure(float exposure);

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
	void updateImage();

	std::vector<Imf::Rgba> hdrImage;

	int		widgetWidth,	widgetHeight;
	int		imageWidth,		imageHeight;
	float	cameraPanX,		cameraPanY;
	float	lastMouseX,		lastMouseY;

	float exposure;

	float zoomFactor;
	float widgetAspectRatio;

	bool mousePressed;

	QLabel& pixelInfoLabel;
	QPushButton& zoomButton;

	GLuint textureId;
	GLuint vtxBuf;
	GLuint uvBuf;
	GLuint matrixLocationId;
	GLuint textureLocationId;
	QOpenGLVertexArrayObject vao;
};

#endif