#pragma once
#include "gli/gli.hpp"
#include <QtOpenGL/QGlWidget>

class drawTexWidget : public QGLWidget
{
public:
	void doFBOstuff();
	void setUpTexture();

	void paintGL();
	bool checkShaderCompileError(GLuint shaderID);
	void initializeGL();
	void setVertDataToGL(float offset);
	void sendDataToOpenGL();

	void setUpShaderProgram();

private:

	gli::texture2D *texture;

	GLuint elementsBuffer;
	GLuint verticesBuffer;
	GLuint textureBuffer;
	GLuint colorBuffer;
	GLuint shaderProgram;
};
