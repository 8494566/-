#ifndef DANMUINTERFACE_H
#define DANMUINTERFACE_H
#include "LibraryManagers/Plugin/Plugin.h"

class QWidget;
class CustomGLWidget;

class DanMuInterface : public Plugin
{
public:

	virtual CustomGLWidget* createCustomGLWidget(QWidget* parent = nullptr) = 0;

};
#endif