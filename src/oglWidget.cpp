#include "oglWidget.hpp"

void tonemap(	const std::vector<Imf::Rgba>& hdrImg,
				std::vector<std::array<uint8_t, 4>>& ldrImg,
				const int w, const int h) {
	for(auto j = 0; j < h; ++j) {
		for(auto i = 0; i < w; ++i) {
			const auto buf_idx = i + j*w;
			ldrImg[buf_idx][1] = (uint8_t)(255*std::min(hdrImg[buf_idx].g, (half)1.0f));
			ldrImg[buf_idx][0] = (uint8_t)(255*std::min(hdrImg[buf_idx].r, (half)1.0f));
			ldrImg[buf_idx][2] = (uint8_t)(255*std::min(hdrImg[buf_idx].b, (half)1.0f));
			ldrImg[buf_idx][3] = (uint8_t)(255*std::min(hdrImg[buf_idx].a, (half)1.0f));
		}
	}
}


OGLWidget::OGLWidget(QLabel& pixelPositionLabel) :	QOpenGLWidget{},
													cameraPanX{0},
													cameraPanY{0},
													zoomFactor{1},
													mousePressed{false},
													pixelPositionLabel{pixelPositionLabel} {};

void OGLWidget::changeImage(const std::vector<Imf::Rgba>& img,
							const int w, const int h) {
	imageWidth = w;
	imageHeight = h;
	hdrImage = img;

	std::vector<std::array<uint8_t, 4>> ldrImg{(size_t)w*h};
	tonemap(img, ldrImg, w, h);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ldrImg.data());
}


void OGLWidget::initializeGL() {
	initializeOpenGLFunctions();
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
	glEnable( GL_TEXTURE_2D );

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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

	if (x < 0 || y < 0 || x >= imageWidth || y >= imageHeight) {
		pixelPositionLabel.setText("Cursor not on image");	
	} else {
		const auto pix = hdrImage[x + y*imageWidth];

		std::stringstream stream;
		stream << std::fixed << std::setprecision(4);
		stream << "x: " << x << "\ty: " << y << "\t";
		stream << "    r: " << pix.r << "    g: " << pix.g << "    b: " << pix.b << "    a: " << pix.a;
		
		pixelPositionLabel.setText(stream.str().c_str());
	}
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