#include "oglWidget.hpp"

void OGLWidget::initializeGL() {
	std::cout << "Hello OpenGL!" << std::endl;
	initializeOpenGLFunctions();
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
	glEnable( GL_TEXTURE_2D );

	std::vector<std::array<GLubyte, 4>> img(256*256, {255, 0, 255, 255});
	for(auto j = 0; j < 256; ++j) {
		for(auto i = 0; i < 256; ++i) {
			img[i+j*256] = {i, j, 0, 255};
		}
	}
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, (GLuint)NULL);
}

void OGLWidget::resizeGL(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, h, 0, -1, +1);
	glMatrixMode( GL_MODELVIEW ); 
	glLoadIdentity();
}

void OGLWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glBegin( GL_QUADS ); 
		glTexCoord2f( 0.f, 0.f ); glVertex2f(   0,   0 );
		glTexCoord2f( 1.f, 0.f ); glVertex2f( 512,   0 );
		glTexCoord2f( 1.f, 1.f ); glVertex2f( 512, 512 );
		glTexCoord2f( 0.f, 1.f ); glVertex2f(   0, 512 );
	glEnd();
}