#include "Qt/qapplication.h"

#include "drawCanvasWidget.h"
#include "drawTexWidget.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	DrawCanvasWidget canvas;
	canvas.show();

	drawTexWidget texWidget;
	texWidget.show();

	return app.exec();

}