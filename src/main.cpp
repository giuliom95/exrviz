#include <iostream>
#include <vector>
#include <array>
#include <cstdint>

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
	oglWidget->setMouseTracking(true);

	mainLayout->addWidget(oglWidget);
	window.setLayout(mainLayout);
	window.show();

	int img_w, img_h;
	std::vector<Imf::Rgba> img{0};
	read_exr(argv[1], img, img_w, img_h);

	std::vector<std::array<uint8_t, 4>> ldr_img{(size_t)img_w*img_h};
	for(auto j = 0; j < img_h; ++j) {
		for(auto i = 0; i < img_w; ++i) {
			const auto buf_idx = i + j*img_w;
			ldr_img[buf_idx][0] = (uint8_t)(255*std::min(img[buf_idx].r, (half)1.0f));
			ldr_img[buf_idx][1] = (uint8_t)(255*std::min(img[buf_idx].g, (half)1.0f));
			ldr_img[buf_idx][2] = (uint8_t)(255*std::min(img[buf_idx].b, (half)1.0f));
			ldr_img[buf_idx][3] = (uint8_t)(255*std::min(img[buf_idx].a, (half)1.0f));
			//std::cout << img[buf_idx].r << " " << img[buf_idx].g << " " << img[buf_idx].b << " " << img[buf_idx].a << " -> ";
			//std::cout << (int)ldr_img[buf_idx][0] << " " << (int)ldr_img[buf_idx][1] << " " << (int)ldr_img[buf_idx][2] << " " << (int)ldr_img[buf_idx][3] << std::endl;
		}
	}

	oglWidget->changeImage(ldr_img, img_w, img_h);

	window.resize(img_w, img_h);

	return app.exec();
}

