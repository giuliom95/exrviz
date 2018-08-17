#include <iostream>
#include <vector>
#include <array>

#include <ImfRgbaFile.h>
#include <ImfRgba.h>

#include <QtWidgets>

#include "oglWidget.hpp"

void read_exr(	const char* fileName,
				std::vector<Imf::Rgba>& image,
				int& width, int& height) {
	Imf::RgbaInputFile file(fileName);
	Imath::Box2i dw = file.dataWindow();
	width  = dw.max.x - dw.min.x + 1;
	height = dw.max.y - dw.min.y + 1;
	image.resize(height * width);
	file.setFrameBuffer(image.data() - dw.min.x - dw.min.y * width, 1, width);
	file.readPixels(dw.min.y, dw.max.y);
}


int main(int argc, char** argv) {
	QApplication app(argc, argv);
	QWidget window;
	window.resize(320, 240);
	window.setWindowTitle("Hello Qt5!");

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->setContentsMargins(0,0,0,0);

	OGLWidget* oglWidget = new OGLWidget;
	
	mainLayout->addWidget(oglWidget);
	window.setLayout(mainLayout);
	window.show();

	return app.exec();
}

