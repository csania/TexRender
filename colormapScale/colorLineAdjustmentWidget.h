#ifndef COLORLINEADJUSTMENTWIDGET_H
#define COLORLINEADJUSTMENTWIDGET_H

#include "Qt/qwidget.h"
#include "Qt/qpainter.h"
#include "QtGui/qevent.h"

//#include <QtWidgets/QWidget>
//#include <QPainter>
//#include <QPaintEvent>
//#include <QMouseEvent>

#include "vector.h"

class ColorLineAdjustmentWidget : public QWidget
{
public:
	ColorLineAdjustmentWidget(QWidget *parent = nullptr);

	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void resizeEvent(QResizeEvent *resize) override;

protected:

	void setColorAdjustCoef();

	void resetAdjustmentWidget();

	bool mousePressed;
	int initialHeight;
	int initialWidth;
};

#endif // !COLORLINEADJUSTMENTWIDGET_H