#ifndef DANMUMANAGERINTERFACE_H
#define DANMUMANAGERINTERFACE_H

#include "Interface/DanMuInterface.h"

class CustomGLWidget;

class DanMuManagerInterface : public DanMuInterface
{
public:

	virtual CustomGLWidget* createCustomGLWidget(QWidget* parent = nullptr);

};
#endif