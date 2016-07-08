#include "colorLineAdjustmentWidget.h"

Vector pointA;
Vector pointB;
Vector adjustCoefs;


ColorLineAdjustmentWidget::ColorLineAdjustmentWidget(QWidget *parent) : QWidget(parent)
{
	mousePressed = false;

	pointA = Vector(0.25 * width(), 0.75 * height(), 0.0);
	pointB = Vector(0.75 * width(), 0.25 * height(), 0.0);

	setColorAdjustCoef();
}

void ColorLineAdjustmentWidget::setColorAdjustCoef()
{
	
	adjustCoefs.x = ((height() - pointA.y) / height()) / (pointA.x / width());
	adjustCoefs.y = ((pointA.y - pointB.y) / height()) / ((pointB.x - pointA.x) / width());
	adjustCoefs.z = (pointB.y / height()) / ((width() - pointB.x) / width());

	Vector normalizedPointA = Vector(pointA.x / width(), pointA.y / height(), 0.0);
	Vector normalizedPointB = Vector(pointB.x / width(), pointB.y / height(), 0.0);

}

void ColorLineAdjustmentWidget::resetAdjustmentWidget()
{
	initialHeight = height();
	initialWidth = width();

	pointA = Vector(0.25 * width(), 0.75 * height(), 0.0);
	pointB = Vector(0.75 * width(), 0.25 * height(), 0.0);
}

void ColorLineAdjustmentWidget::mousePressEvent(QMouseEvent * event)
{
	mousePressed = true;
	Vector mousePos;
	mousePos.x = this->mapFromGlobal(QCursor::pos()).x();
	mousePos.y = this->mapFromGlobal(QCursor::pos()).y();

	double distToA = sqrt((mousePos.x - pointA.x) * (mousePos.x - pointA.x) + (mousePos.y - pointA.y) * (mousePos.y - pointA.y));
	double distToB = sqrt((mousePos.x - pointB.x) * (mousePos.x - pointB.x) + (mousePos.y - pointB.y) * (mousePos.y - pointB.y));

	if (distToA < distToB) {
		pointA.z = 1.0;
	}
	else {
		pointA.z = 0.0;
	}
	update();
}

void ColorLineAdjustmentWidget::mouseMoveEvent(QMouseEvent * event)
{
	if (mousePressed) {

		int mouseXCoord = this->mapFromGlobal(QCursor::pos()).x();
		int mouseYCoord = this->mapFromGlobal(QCursor::pos()).y();

		if (pointA.z == 1.0) {
			if (mouseXCoord < pointB.x && mouseXCoord > 0 && mouseYCoord > 0 && mouseYCoord < height()) {
				pointA.x = mouseXCoord;
				pointA.y = mouseYCoord;
			}
		}
		else {
			if (mouseXCoord > pointA.x && mouseXCoord < width() && mouseYCoord > 0 && mouseYCoord < height()) {
				pointB.x = mouseXCoord;
				pointB.y = mouseYCoord;
			}
		}

		setColorAdjustCoef();
		update();
	}
}

void ColorLineAdjustmentWidget::mouseReleaseEvent(QMouseEvent * event)
{
	mousePressed = false;
	update();
}

void ColorLineAdjustmentWidget::resizeEvent(QResizeEvent * resize)
{
	double oldHeight = resize->oldSize().height();
	double oldWidth = resize->oldSize().width();

	if (oldHeight == -1 && oldWidth == -1) {
		resetAdjustmentWidget();
		oldHeight = initialHeight;
		oldWidth = initialWidth;
	}

	double heightDif = (double)height() / oldHeight;
	double widthDif = (double)width() / oldWidth;

	pointA.x = pointA.x * widthDif;
	pointA.y = pointA.y * heightDif;

	pointB.x = pointB.x * widthDif;
	pointB.y = pointB.y * heightDif;

}

void ColorLineAdjustmentWidget::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::RenderHint::HighQualityAntialiasing, true);

	QPen pen(Qt::black);
	pen.setWidth(2);
	painter.setPen(pen);

	painter.drawLine(0, height(), width(), 0);
	painter.drawLine(0, height(), pointA.x, pointA.y);
	painter.drawLine(pointA.x, pointA.y, pointB.x, pointB.y);
	painter.drawLine(pointB.x, pointB.y, width(), 0);

	painter.drawEllipse(QPoint(pointA.x, pointA.y), 8, 8);
	painter.drawEllipse(QPoint(pointB.x, pointB.y), 8, 8);

	painter.end();
}