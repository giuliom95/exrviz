#include "oglWidget.hpp"

float clamp(float v, float lo, float hi) {
	if(v < lo)
		return lo;
	if(v > hi)
		return hi;
	return v;
}

/*
 *	This tonemapping function is almost the one decribed here:
 *	http://www.openexr.com/using.html
 *	(The knee function has been ignored for now)
 */
void tonemap(	const std::vector<Imf::Rgba>& hdrImg,
				std::vector<std::array<uint8_t, 3>>& ldrImg,
				const int w, const int h) {

	const auto exposure = 0.0f;
	const auto a = std::pow(2, exposure + 2.47393f);
	const auto b = 84.66f;
	const auto invGamma = 1 / 2.2f;

	for(auto j = 0; j < h; ++j) {
		for(auto i = 0; i < w; ++i) {
			auto& ldrPix = ldrImg[i + j*w];
			const auto& hdrPix = hdrImg[i + j*w];
			ldrPix[0] = (uint8_t)(clamp(b*std::pow(a*hdrPix.r, invGamma), 0, 255));
			ldrPix[1] = (uint8_t)(clamp(b*std::pow(a*hdrPix.g, invGamma), 0, 255));
			ldrPix[2] = (uint8_t)(clamp(b*std::pow(a*hdrPix.b, invGamma), 0, 255));
		}
	}
}

OGLWidget::OGLWidget(	QLabel& pixelInfoLabel,
						QPushButton& zoomButton) :	QOpenGLWidget{},
													cameraPanX{0},
													cameraPanY{0},
													zoomFactor{1},
													mousePressed{false},
													pixelInfoLabel{pixelInfoLabel},
													zoomButton{zoomButton} {};

void OGLWidget::changeImage(const std::vector<Imf::Rgba>& img,
							const int w, const int h) {
	imageWidth = w;
	imageHeight = h;
	hdrImage = img;

	std::vector<std::array<uint8_t, 3>> ldrImg{(size_t)w*h};
	tonemap(img, ldrImg, w, h);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, ldrImg.data());
}


void OGLWidget::setZoom(float zf, float x, float y) {
	cameraPanX += x / zoomFactor;
	cameraPanY += y / zoomFactor;

	zoomFactor = zf;
	zoomFactor = std::max(0.5f, zoomFactor);
	zoomFactor = std::min(10.f, zoomFactor);

	cameraPanX -= x / zoomFactor;
	cameraPanY -= y / zoomFactor;

	std::stringstream stream;
	stream << std::fixed << std::setprecision(1);
	stream << (100 * zoomFactor) << "%";
	zoomButton.setText(stream.str().c_str());

	update();
}


void OGLWidget::initializeGL() {
	initializeOpenGLFunctions();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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
		pixelInfoLabel.setText("Cursor not on image");	
	} else {
		const auto pix = hdrImage[x + y*imageWidth];

		std::stringstream stream;
		stream << std::fixed << std::setprecision(4);
		stream << "x: " << x << "\ty: " << y << "\t";
		stream << "    r: " << pix.r << "    g: " << pix.g << "    b: " << pix.b << "    a: " << pix.a;
		
		pixelInfoLabel.setText(stream.str().c_str());
	}
}

void OGLWidget::wheelEvent(QWheelEvent *event) {
	const auto mousePos = event->pos();
	const auto scroll = event->angleDelta().y() / 120.f;
	const auto newZoomFactor = zoomFactor + 0.5*scroll;

	setZoom(newZoomFactor, mousePos.x(), mousePos.y());
}