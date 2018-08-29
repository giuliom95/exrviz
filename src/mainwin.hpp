#ifndef MAINWIN_HPP
#define MAINWIN_HPP

#include <vector>

#include <ImfRgbaFile.h>
#include <ImfRgba.h>

#include <QtWidgets>

#include "oglWidget.hpp"


class ExrVizMainWindow : public QWidget {
	Q_OBJECT
public:
	explicit ExrVizMainWindow();
	
	void changeImage(	const std::vector<Imf::Rgba>& inputHdrImage,
						const int imageWidth, const int imageHeight);

public slots:
	void handleZoomButton();
	void handleExposureChange(int);

private:
	QPushButton*	zoomButton;
	QLabel*			exposureLabel;
	QSlider*		exposureSlider;
	OGLWidget*		oglWidget;
};

#endif