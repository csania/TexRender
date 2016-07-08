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
	int textureWidth = 0;
	int textureHeight = 0;

	if (TextureLoader::Instance()->loadDDsTex("D://AdR//LonelyEaton//Resources//1250//ER01I.dds")) {
		texture = TextureLoader::Instance()->getLoadedTexture();
		spFit = (int *)(*texture)[0].data();
		textureWidth = (*texture)[0].dimensions().x;
		textureHeight = (*texture)[0].dimensions().y;
	}

	unsigned int tex2[] = {
		0xff0000ff,0xff0000ff,0xff00ff00,0xff0000ff,0xff0000ff,0xff0000ff,0xff0000ff,0xffFF00ff
	};

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, spFit);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// was GL_NEAREST
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLint samplerLoc = glGetUniformLocation(shaderProgram, "TexSampler");
	glUniform1i(samplerLoc, 0);

}



void drawTexWidget::sendDataToOpenGL()
{
	/*
	GLfloat verts[]
	{
		-1.0f, -0.0f,
		-1.0f, -1.0f,
		-0.0f, -1.0f,
		   		  
		-0.0f, -1.0f,
		-0.0f, -0.0f,
		-1.0f, -0.0f,
	};
	*/

	GLfloat verts[]
	{
		-1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, -1.0f
	};

	//GLuint myBufferID;
	glGenBuffers(1, &verticesBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
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

	/*
	GLfloat color[] = { 1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.2f, 1.0f,
		0.3f, 0.0f, 1.0f
	};

	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	*/

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

void drawTexWidget::paintGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

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