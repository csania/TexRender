#pragma once

#include <string>
#include <QtOpenGL/QGlWidget>
#include "gli/gli.hpp"

class DrawCanvasWidget : public QGLWidget
{
public:
	void paintGL();
	void initializeGL();
	void sendDataToOpenGL();

	unsigned int setupPointDrawShader();
	std::string readShaderSource(std::string location);
	bool checkShaderCompileError(GLuint shaderID);

private:

	unsigned int shaderProgram;
	
};