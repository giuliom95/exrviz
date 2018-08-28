#include "oglWidget.hpp"

OGLWidget::OGLWidget(	QLabel& pixelInfoLabel,
						QPushButton& zoomButton) :	QOpenGLWidget{},
													cameraPanX{0},
													cameraPanY{0},
													exposure{0.0f},
													zoomFactor{1},
													mousePressed{false},
													pixelInfoLabel{pixelInfoLabel},
													zoomButton{zoomButton},
													vao{} {
	QSurfaceFormat format;
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(4,1);
	setFormat(format);
};

void OGLWidget::changeImage(const std::vector<Imf::Rgba>& img,
							const int w, const int h) {
	imageWidth = w;
	imageHeight = h;
	hdrImage = img;

	const GLfloat vtxBufData[] = {
		0.0f, 0.0f,
		   w, 0.0f,
		0.0f,    h,
		   w,    h
	};
	glGenBuffers(1, &vtxBuf);
	glBindBuffer(GL_ARRAY_BUFFER, vtxBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtxBufData), vtxBufData, GL_STATIC_DRAW);

	const GLfloat uvBufData[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f
	};
	glGenBuffers(1, &uvBuf);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvBufData), uvBufData, GL_STATIC_DRAW);

	updateImage();
}


void OGLWidget::changeExposure(float e) {
	exposure = e;
	updateImage();
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
	glEnable(GL_TEXTURE_2D);
	
	vao.create();
	vao.bind();

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Create shaders
	const auto vtxShaderId = glCreateShader(GL_VERTEX_SHADER);
	const auto fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	GLint result = GL_FALSE;
	int infoLogLen;

	// Compile Vertex Shader
	const std::string vtxShaderCode = "#version 330 core \n"
	"layout(location = 0) in vec2 vtx_pos;"
	"layout(location = 1) in vec2 vtx_uv;"
	"out vec2 uv;"
	"uniform mat4 proj;"
	"void main(){"
		"gl_Position.xy = vtx_pos;"
		"gl_Position.z = 0.0f;"
		"gl_Position.w = 1.0f;"
		"gl_Position = gl_Position * proj;"
		"uv = vtx_uv;"
	"}";
	const auto* vtxShaderCodePtr = vtxShaderCode.c_str();
	glShaderSource(vtxShaderId, 1, &vtxShaderCodePtr, NULL);
	glCompileShader(vtxShaderId);

	// Check Vertex Shader
	glGetShaderiv(vtxShaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vtxShaderId, GL_INFO_LOG_LENGTH, &infoLogLen);
	if ( infoLogLen > 0 ){
		std::vector<char> msg(infoLogLen+1);
		glGetShaderInfoLog(vtxShaderId, infoLogLen, NULL, msg.data());
		std::cout << std::string(msg.data()) << std::endl;
	}

	// Compile Frag Shader
	const std::string fragShaderCode = "#version 330 core\n"
	"in vec2 uv;"
	"out vec3 color;"
	"uniform sampler2D image;"
	"void main(){"
		"float exposure = 0.0f;"
		"float a = pow(2, exposure + 2.47393f);"
		"float b = 84.66f / 255.0f;"
		"vec3 invGamma = vec3(1 / 2.2f);"
		"vec3 hdr = texture(image, uv).rgb;"
		"color = clamp(b*pow(a*hdr, invGamma), 0.0f, 1.0f);"
	"}";
	const auto* fragShaderCodePtr = fragShaderCode.c_str();
	glShaderSource(fragShaderId, 1, &fragShaderCodePtr, NULL);
	glCompileShader(fragShaderId);

	// Check Frag Shader
	glGetShaderiv(fragShaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragShaderId, GL_INFO_LOG_LENGTH, &infoLogLen);
	if ( infoLogLen > 0 ){
		std::vector<char> msg(infoLogLen+1);
		glGetShaderInfoLog(fragShaderId, infoLogLen, NULL, msg.data());
		std::cout << std::string(msg.data()) << std::endl;
	}

	// Link the program
	GLuint progId = glCreateProgram();
	glAttachShader(progId, vtxShaderId);
	glAttachShader(progId, fragShaderId);
	glLinkProgram(progId);

	// Check the program
	glGetProgramiv(progId, GL_LINK_STATUS, &result);
	glGetProgramiv(progId, GL_INFO_LOG_LENGTH, &infoLogLen);
	if ( infoLogLen > 0 ){
		std::vector<char> msg(infoLogLen+1);
		glGetProgramInfoLog(progId, infoLogLen, NULL, msg.data());
		std::cout << std::string(msg.data()) << std::endl;
	}
	
	glDetachShader(progId, vtxShaderId);
	glDetachShader(progId, fragShaderId);
	
	glDeleteShader(vtxShaderId);
	glDeleteShader(fragShaderId);

	glUseProgram(progId);

	matrixLocationId = glGetUniformLocation(progId, "proj");
	textureLocationId = glGetUniformLocation(progId, "image");
}

void OGLWidget::resizeGL(int w, int h) {
	widgetWidth = w;
	widgetHeight = h;
	widgetAspectRatio = (float)w / h;
}

void OGLWidget::paintGL() {
	
	const auto invZoom = 1 / zoomFactor;
	const auto left = cameraPanX;
	const auto right = cameraPanX + widgetWidth*invZoom;
	const auto bottom = cameraPanY + widgetHeight*invZoom;
	const auto top = cameraPanY;
	const auto A = 2.0f / (right - left);
	const auto B = 2.0f / (top - bottom);
	const auto C = -1;
	const auto tx = -(right + left) / (right - left);
	const auto ty = -(top + bottom) / (top - bottom);
	const auto tz = 0;
	const std::array<GLfloat, 16> mat{A, 0, 0, tx, 0, B, 0, ty, 0, 0, C, tz, 0, 0, 0, 1};
	glUniformMatrix4fv(matrixLocationId, 1, GL_FALSE, mat.data());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glUniform1i(textureLocationId, 0);

	glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vtxBuf);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuf);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
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

void OGLWidget::updateImage() {
	
	std::vector<std::array<float, 3>> data{(size_t)imageWidth*imageHeight};
	for(auto i = 0; i < imageWidth*imageHeight; ++i) {
		data[i][0] = (float)hdrImage[i].r;
		data[i][1] = (float)hdrImage[i].g;
		data[i][2] = (float)hdrImage[i].b;
	}
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_FLOAT, data.data());
	update();
}