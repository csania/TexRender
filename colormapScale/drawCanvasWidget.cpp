#include "GL/glew.h"
#include "drawCanvasWidget.h"

#include "TextureLoader.h"


void DrawCanvasWidget::initializeGL()
{
	glewInit();
	sendDataToOpenGL();

	std::string vsLocation = "D:\\GitRep\\TexRender\\Shaders\\colorScale_vs.glsl";
	std::string fsLocation = "D:\\GitRep\\TexRender\\Shaders\\colorScale_fs.glsl";

	shaderProgram = TextureLoader::Instance()->setupPointDrawShader(vsLocation, fsLocation);
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