#include "oglWidget.hpp"

OGLWidget::OGLWidget() :	QOpenGLWidget{},
							cameraPanX{0},
							cameraPanY{0},
							zoomFactor{1},
							mousePressed{false} {};

void OGLWidget::changeImage(const std::vector<std::array<uint8_t, 4>>& img, 
							const int w, const int h) {
	imageWidth = w;
	imageHeight = h;
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data());
}


void OGLWidget::initializeGL() {
	std::cout << "Hello OpenGL!" << std::endl;
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
	//std::cout << event->x() << " " << event->y() << " ";
	//std::cout << cameraPanX << " " << cameraPanY << std::endl;
}

void OGLWidget::mouseReleaseEvent(QMouseEvent* event) {
	mousePressed = false;
	update();
}

void OGLWidget::mouseMoveEvent(QMouseEvent* event) {
	if(mousePressed) {
		const auto mousePos = event->screenPos();
		const int curMouseX = mousePos.x();
		const int curMouseY = mousePos.y(); 

		// Movement speed must be inversely proportional to zoom factor
		const auto invZoom = 1 / zoomFactor;
		cameraPanX -= invZoom*(curMouseX - lastMouseX);
		cameraPanY -= invZoom*(curMouseY - lastMouseY);
		lastMouseX = curMouseX;
		lastMouseY = curMouseY;

		update();
	}
}

void OGLWidget::wheelEvent(QWheelEvent *event) {
	const auto mousePos = event->pos();
	const auto scroll = event->angleDelta().y();
	const auto direction = (scroll > 0) - (scroll < 0);
	
	zoomFactor += 0.5*direction;
	zoomFactor = std::max(0.5f, zoomFactor);
	zoomFactor = std::min(10.f, zoomFactor);

	//std::cout << zoomFactor << std::endl;
	update();
}