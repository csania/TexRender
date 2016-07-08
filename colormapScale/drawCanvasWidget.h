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

private:

	unsigned int shaderProgram;
	
};