#include "DanMuManager.h"
#include "customglwidget.h"


CustomGLWidget* DanMuManagerInterface::createCustomGLWidget(QWidget* parent)
{
	return new CustomGLWidget(parent);
}

