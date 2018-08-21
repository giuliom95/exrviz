#include <iostream>

#include <ImfRgbaFile.h>
#include <ImfRgba.h>

#include <QtWidgets>

#include "mainwin.hpp"

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

	ExrVizMainWindow win;	
	win.show();

	int img_w, img_h;
	std::vector<Imf::Rgba> img{0};
	read_exr(argv[1], img, img_w, img_h);
	win.changeImage(img, img_w, img_h);

	return app.exec();
}