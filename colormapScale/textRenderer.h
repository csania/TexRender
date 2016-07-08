#pragma once

#include "Vector/vector.h"
#include <string>
#include <QtGui/QPixMap>

class drawTexWidget;

class TextRenderer {

public:

	TextRenderer();
	~TextRenderer();

	void render(); ///draw everything told about

	void addText3D(std::string text, Vector positionIn3DSpace);
	void addText2D(std::string text, Vector screenPosition);

	void addPixMap3D(QPixmap map, Vector position);

private:
	drawTexWidget* drawWidget;

};