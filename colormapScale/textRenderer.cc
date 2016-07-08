#include "GL/glew.h"
#include "textRenderer.h"

#include "Qt/qpainter.h"
#include "Qt/qpicture.h"
#include "Qt/qpixmap.h"
#include "Qt/qimage.h"


TextRenderer::TextRenderer()
{

}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::addText2D(std::string text, Vector screenPosition)
{
	QPixmap pMap(200, 200);
	QPainter painter;
	painter.begin(&pMap);

	QFont font = painter.font();
	font.setPixelSize(48);
	painter.setFont(font);

	const QRect rectangle = QRect(0, 0, 100, 50);
	QRect boundingRect;
	painter.drawText(rectangle, 0, "Hello", &boundingRect);
}
