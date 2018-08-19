#include "oglWidget.hpp"

OGLWidget::OGLWidget(QLabel& pixelPositionLabel) :	QOpenGLWidget{},
													cameraPanX{0},
													cameraPanY{0},
													zoomFactor{1},
													mousePressed{false},
													pixelPositionLabel{pixelPositionLabel} {};

void OGLWidget::changeImage(const std::vector<std::array<uint8_t, 4>>& img, 
							const int w, const int h) {
	imageWidth = w;
	imageHeight = h;
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data());
}


void OGLWidget::initializeGL() {
	initializeOpenGLFunctions();
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
	glEnable( GL_TEXTURE_2D );

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Just a placeholder
	const std::vector<std::array<uint8_t, 4>> img{256*256, {255,0,255,255}};
	changeImage(img, 256, 256);
}

void OGLWidget::resizeGL(int w, int h) {
	widgetWidth = w;
	widgetHeight = h;
	widgetAspectRatio = (float)w / h;
}

void OGLWidget::paintGL() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	const auto invZoom = 1 / zoomFactor;
	glOrtho(
		cameraPanX,
		cameraPanX + widgetWidth*invZoom,
		cameraPanY + widgetHeight*invZoom,
		cameraPanY, 
		-1, +1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glBegin(GL_QUADS); 
		glTexCoord2f(0.f, 0.f); glVertex2f(0, 0);
		glTexCoord2f(1.f, 0.f); glVertex2f(imageWidth, 0);
		glTexCoord2f(1.f, 1.f); glVertex2f(imageWidth, imageHeight);
		glTexCoord2f(0.f, 1.f); glVertex2f(0, imageHeight);
	glEnd();
}

void OGLWidget::mousePressEvent(QMouseEvent* event) {
	mousePressed = true;
	const auto mousePos = event->screenPos();
	lastMouseX = (int)mousePos.x();
	lastMouseY = (int)mousePos.y();
}

void OGLWidget::mouseReleaseEvent(QMouseEvent* event) {
	mousePressed = false;
	update();
}

void OGLWidget::mouseMoveEvent(QMouseEvent* event) {
	const auto invZoom = 1 / zoomFactor;

	if(mousePressed) {
		const auto mousePos = event->screenPos();
		const int curMouseX = mousePos.x();
		const int curMouseY = mousePos.y(); 

		cameraPanX -= invZoom*(curMouseX - lastMouseX);
		cameraPanY -= invZoom*(curMouseY - lastMouseY);
		lastMouseX = curMouseX;
		lastMouseY = curMouseY;

		update();
	}

	const auto mousePos = event->pos();
	const int x = invZoom*mousePos.x() + cameraPanX;
	const int y = invZoom*mousePos.y() + cameraPanY;

	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << "x: " << x << "    y: " << y;
	
	pixelPositionLabel.setText(stream.str().c_str());
}

void OGLWidget::wheelEvent(QWheelEvent *event) {
	const auto mousePos = event->pos();
	const auto scroll = event->angleDelta().y() / 120.f;
	
	cameraPanX += mousePos.x() / zoomFactor;
	cameraPanY += mousePos.y() / zoomFactor;

	zoomFactor += 0.5*scroll;
	zoomFactor = std::max(0.5f, zoomFactor);
	zoomFactor = std::min(10.f, zoomFactor);

	cameraPanX -= mousePos.x() / zoomFactor;
	cameraPanY -= mousePos.y() / zoomFactor;

	update();
}