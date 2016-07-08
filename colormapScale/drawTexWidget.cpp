#include "GL/glew.h"

#include "TextureLoader.h"
#include "drawTexWidget.h"

const GLchar* vertexSource =
"#version 120\n"
"attribute vec2 vertices;"
"attribute vec2 uvs;"

"varying vec2 interpolatedUVs;"

"void main()"
"{"
"    interpolatedUVs = uvs;"
"    gl_Position = vec4(vertices, 0.0, 1.0);"
"}";

const GLchar* fragmentSource =
"#version 120\n"
"varying vec2 interpolatedUVs;"
"uniform sampler2D TexSampler;"
"void main()"
"{"
"    gl_FragColor = texture2D(TexSampler, interpolatedUVs);"
"}";

void drawTexWidget::setUpTexture()
{
	GLuint tex;
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	int* spFit = nullptr;
	int textureWidth = 4;
	int textureHeight = 2;

	unsigned int tex2[] = {
		0xff0000ff,0xff0000ff,0xff00ff00,0xff0000ff,0xff0000ff,0xff0000ff,0xff0000ff,0xffFF00ff
	};

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex2);

	QImage qIm1 = QImage(400, 400, QImage::Format_RGB32);
	QPainter painter;
	painter.begin(&qIm1);

	QFont font = painter.font();
	font.setPixelSize(55);
	painter.setFont(font);

	const QRect rectangle = QRect(20, 20, 200, 200);
	QRect boundingRect;
	painter.drawText(rectangle, 0, "Hello", &boundingRect);
	painter.end();
	qIm1 = QGLWidget::convertToGLFormat(qIm1);

	QImage qIm2 = QImage(400, 400, QImage::Format_RGB32);
	painter.begin(&qIm2);
	font = painter.font();
	font.setPixelSize(55);
	painter.setFont(font);
	painter.drawText(rectangle, 0, "Hello", &boundingRect);
	painter.end();
	qIm2 = QGLWidget::convertToGLFormat(qIm2);

	QImage result = QImage(800, 800, QImage::Format_RGB32);
	painter.begin(&result);
	painter.drawImage(0, 0, qIm1);
	painter.drawImage(0, 400, qIm2);
	painter.end();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, result.width(), result.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, result.bits());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// was GL_NEAREST
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLint samplerLoc = glGetUniformLocation(shaderProgram, "TexSampler");
	glUniform1i(samplerLoc, 0);

}

void drawTexWidget::sendDataToOpenGL()
{
	GLfloat verts[]
	{
		-1.0f, -1.0f,
		-1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, -1.0f
	};

	//GLuint myBufferID;
	glGenBuffers(1, &verticesBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), NULL, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, 10000, NULL, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	GLfloat uvs[]
	{
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	//GLuint myBufferID;
	glGenBuffers(1, &textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint indices[]
	{
		0, 1, 2,
		0, 2, 3
	};

	glGenBuffers(1, &elementsBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void drawTexWidget::setUpShaderProgram()
{
	// Create and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	if (checkShaderCompileError(vertexShader) && checkShaderCompileError(fragmentShader)) {

		// Link the vertex and fragment shader into a shader program
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glBindFragDataLocation(shaderProgram, 0, "outColor");
		glLinkProgram(shaderProgram);

		int status = 0;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
		if (GL_FALSE == status) {
			int a = 0;
		}

		glUseProgram(shaderProgram);
	}
}

void drawTexWidget::initializeGL()
{
	glewInit();
	setUpShaderProgram();
	setUpTexture();
	sendDataToOpenGL();
}

void drawTexWidget::setVertDataToGL(float offset)
{
	GLfloat verts[]
	{
		-1.0f + offset, -1.0f + offset,
		-1.0f + offset, 0.0f + offset,
		0.0f + offset, 0.0f + offset,
		0.0f + offset, -1.0f + offset
	};

	int of = 1 + (int)offset;

	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, of * sizeof(verts), sizeof(verts), verts);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void drawTexWidget::paintGL()
{
	glClearColor(0.3f, 0.7f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	setVertDataToGL(0.0f);
	// Draw a rectangle from the 2 triangles using 6 indices
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	setVertDataToGL(1.0f);
	// Draw a rectangle from the 2 triangles using 6 indices
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

bool drawTexWidget::checkShaderCompileError(GLuint shaderID)
{
	GLint compiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		// Compile failed, store log and return false
		int length = 0;
		//logString = "";
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
		if (length > 0) {
			char * c_log = new char[length];
			int written = 0;
			glGetShaderInfoLog(shaderID, length, &written, c_log);
			auto logString = c_log;
			delete[] c_log;
		}
		return false;
	}
	return true;
}