#include "mainwin.hpp"

ExrVizMainWindow::ExrVizMainWindow() : QWidget() {
	resize(600, 600);
	setWindowTitle("exrviz");

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->setContentsMargins(0,0,0,0);
	mainLayout->setSpacing(0);

	QWidget* topBarLayoutContainer = new QWidget;	// This is used to give a fixed height to the layout
	topBarLayoutContainer->setFixedHeight(25);
	QHBoxLayout* topBarLayout = new QHBoxLayout;
	topBarLayout->setContentsMargins(5,0,5,0);
	exposureLabel = new QLabel("Exposure:  +0.00");
	exposureLabel->setFixedWidth(115);
	exposureSlider = new QSlider(Qt::Horizontal);
	exposureSlider->setMinimum(-50);
	exposureSlider->setMaximum(+50);
	exposureSlider->setTickPosition(QSlider::TicksBothSides);
	exposureSlider->setFixedWidth(400);
	exposureSlider->setValue(0);
	topBarLayout->addWidget(exposureLabel);
	topBarLayout->addWidget(exposureSlider);
	topBarLayout->addStretch();
	topBarLayoutContainer->setLayout(topBarLayout);

	QWidget* bottomBarLayoutContainer = new QWidget;	// This is used to give a fixed height to the layout
	bottomBarLayoutContainer->setFixedHeight(25);
	QHBoxLayout* bottomBarLayout = new QHBoxLayout;
	bottomBarLayout->setContentsMargins(0,0,5,0);
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
	const auto exp = value / 10.0f;

	std::stringstream stream;
	stream << std::fixed << std::setprecision(2);
	stream << "Exposure:  ";
	if (exp >= 0.0f) stream << "+";
	stream << exp;
	exposureLabel->setText(stream.str().c_str());

	oglWidget->changeExposure(exp);
}