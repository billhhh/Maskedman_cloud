#ifndef QTMASKMANDETECT_H
#define QTMASKMANDETECT_H

#include <QtGui/QMainWindow>
#include "ui_qtmaskmandetect.h"

class QtMaskManDetect : public QMainWindow
{
	Q_OBJECT

public:
	QtMaskManDetect(QWidget *parent = 0, Qt::WFlags flags = 0);
	~QtMaskManDetect();

private:
	Ui::QtMaskManDetectClass ui;
};

#endif // QTMASKMANDETECT_H
