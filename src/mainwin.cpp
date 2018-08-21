#include "mainwin.hpp"

ExrVizMainWindow::ExrVizMainWindow() : QWidget() {
	resize(500, 500);
	setWindowTitle("exrviz");

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->setContentsMargins(0,0,0,0);

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

	mainLayout->addWidget(oglWidget);
	mainLayout->addWidget(bottomBarLayoutContainer);
	setLayout(mainLayout);
}