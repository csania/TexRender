#include "GL/glew.h"
#include "drawCanvasWidget.h"

#include <fstream>
#include <sstream>


void DrawCanvasWidget::initializeGL()
{
	glewInit();
	sendDataToOpenGL();
	shaderProgram = setupPointDrawShader();
	glUseProgram(shaderProgram);
}

void DrawCanvasWidget::sendDataToOpenGL()
{
	GLfloat verts[]
	{
		-0.97f, -0.35f,
		-0.97f, -0.70f,
		-0.80f, -0.70f,

		-0.80f, -0.70f,
		-0.80f, -0.35f,
		-0.97f, -0.35f,
	};

	GLuint myBufferID;
	glGenBuffers(1, &myBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, myBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

}

void DrawCanvasWidget::paintGL()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, width(), height());

	int selectedScheme = 2;
	std::string tex = "Texture";

	GLint currentScheme = glGetUniformLocation(shaderProgram, "CurrentScheme");

	glUniform1i(currentScheme, selectedScheme);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

unsigned int DrawCanvasWidget::setupPointDrawShader()
{
	bool shadersSupported = (glCreateShader != nullptr);
	GLuint shaderProgId = 0;

	if (shadersSupported) {

		const char *shaderSrc[1];

		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		std::string vsSource = readShaderSource("C:\\Users\\alex.cicenkov\\Documents\\Visual Studio 2015\\Projects\\colormapScale\\Shaders\\colorScale_vs.glsl");
		std::string fsSource = readShaderSource("C:\\Users\\alex.cicenkov\\Documents\\Visual Studio 2015\\Projects\\colormapScale\\Shaders\\colorScale_fs.glsl");

		shaderSrc[0] = vsSource.c_str();
		glShaderSource(vertexShaderID, 1, shaderSrc, 0);
		glCompileShader(vertexShaderID);

		shaderSrc[0] = fsSource.c_str();
		glShaderSource(fragmentShaderID, 1, shaderSrc, 0);
		glCompileShader(fragmentShaderID);

		if (checkShaderCompileError(vertexShaderID) && checkShaderCompileError(fragmentShaderID)) {

			shaderProgId = glCreateProgram();

			glAttachShader(shaderProgId, vertexShaderID);
			glAttachShader(shaderProgId, fragmentShaderID);

			glLinkProgram(shaderProgId);


			int status = 0;
			glGetProgramiv(shaderProgId, GL_LINK_STATUS, &status);
			if (GL_FALSE == status) {
				// Store log and return false
				int length = 0;
				std::string logString = "";

				glGetProgramiv(shaderProgId, GL_INFO_LOG_LENGTH, &length);

				if (length > 0) {
					char * c_log = new char[length];
					int written = 0;
					glGetProgramInfoLog(shaderProgId, length, &written, c_log);
					logString = c_log;
					delete[] c_log;
				}

				return false;
			}

			GLint linkedVS, linkedFS = 0;
			glGetProgramiv(shaderProgId, GL_LINK_STATUS, &linkedVS);
			glGetProgramiv(shaderProgId, GL_LINK_STATUS, &linkedFS);

			if (!linkedVS || !linkedFS) {
				shaderProgId = 0;
			}
		}
	}
	else {
		std::ostringstream hardwareInfo;
		if (glGetString(GL_VERSION)) {
			hardwareInfo << "\tGL version   = " << glGetString(GL_VERSION) << "\n";
		}
		if (glGetString(GL_VENDOR)) {
			hardwareInfo << "\tGL vendor    = " << glGetString(GL_VENDOR) << "\n";
		}
		if (glGetString(GL_RENDERER)) {
			hardwareInfo << "\tGL renderer  = " << glGetString(GL_RENDERER) << "\n";
		}
		if (glGetString(GL_EXTENSIONS)) {
			hardwareInfo << "\tGL extensions= " << glGetString(GL_EXTENSIONS) << "\n";
		}

	}
	return shaderProgId;
}

std::string DrawCanvasWidget::readShaderSource(std::string location)
{
	std::string content;
	std::ifstream fileIn(location);

	if (fileIn.good()) {
		content.assign((std::istreambuf_iterator<char>(fileIn)),
			std::istreambuf_iterator<char>());
	}

	return content;
}

bool DrawCanvasWidget::checkShaderCompileError(GLuint shaderID)
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