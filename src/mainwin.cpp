#include "mainwin.hpp"

ExrVizMainWindow::ExrVizMainWindow() : QWidget() {
	resize(600, 600);
	setWindowTitle("exrviz");

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->setContentsMargins(0,0,0,0);

	QWidget* topBarLayoutContainer = new QWidget;	// This is used to give a fixed height to the layout
	topBarLayoutContainer->setFixedHeight(30);
	QHBoxLayout* topBarLayout = new QHBoxLayout;
	QLabel* exposureLabel = new QLabel("Exposure: ");
	exposureSlider = new QSlider(Qt::Horizontal);
	exposureSlider->setMinimum(-50);
	exposureSlider->setMaximum(+50);
	exposureSlider->setValue(0);
	topBarLayout->addWidget(exposureLabel);
	topBarLayout->addWidget(exposureSlider);
	topBarLayoutContainer->setLayout(topBarLayout);

	QWidget* bottomBarLayoutContainer = new QWidget;	// This is used to give a fixed height to the layout
	bottomBarLayoutContainer->setFixedHeight(30);
	QHBoxLayout* bottomBarLayout = new QHBoxLayout;
	zoomButton = new QPushButton("100.0%");
	zoomButton->setFixedWidth(70);
	QLabel* pixelInfoLabel = new QLabel("Cursor not on image");
	bottomBarLayout->addWidget(zoomButton);
	bottomBarLayout->addWidget(pixelInfoLabel);
	bottomBarLayoutContainer->setLayout(bottomBarLayout);

	oglWidget = new OGLWidget(*pixelInfoLabel, *zoomButton);
	oglWidget->setMouseTracking(true);

	mainLayout->addWidget(topBarLayoutContainer);
	mainLayout->addWidget(oglWidget);
	mainLayout->addWidget(bottomBarLayoutContainer);
	setLayout(mainLayout);

	connect(zoomButton,		SIGNAL (clicked()),			this, SLOT (handleZoomButton()));
	connect(exposureSlider,	SIGNAL (valueChanged(int)),	this, SLOT (handleExposureChange(int)));
}

void ExrVizMainWindow::handleZoomButton() {
	const auto cx = oglWidget->width() / 2;
	const auto cy = oglWidget->height() / 2;
	oglWidget->setZoom(1.0f, cx, cy);
}

void ExrVizMainWindow::handleExposureChange(int value) {
	oglWidget->changeExposure(value / 10.0f);
}