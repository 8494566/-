#include <QtWidgets/QApplication>
#include <QWidget>
#include <QLabel>
#include "LibraryManagers/Plugin/PluginManager.h"
#include "DanMuInterface/Interface/DanMuInterface.h"
#include "DanMuInterface/CustomglWidget.h"

#ifndef QSTRING_U
#define QSTRING_U
#define u(x) QStringLiteral(x)
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString dir = a.applicationDirPath();

	PluginManager* pm = PluginManager::GetPluginManager();
	pm->loadPlugin(dir.toStdString());//设置exe路径 必须

    QWidget w;
    w.setFixedSize(520, 480);

	DanMuInterface* Interface = (DanMuInterface*)pm->getPlugin("DanMuInterface");
    CustomGLWidget* GLWidget = Interface->createCustomGLWidget(&w);

    QLabel* label = new QLabel(&w);
    label->setGeometry(50, 50, 100, 30);
    label->setText(u("小小污渍,可笑可笑"));

    QImage image;
    image.load(dir + QString("/images/lena2.jpg"));

    GLWidget->setImageData(image);
    GLWidget->move(0, 80);

    w.show();
    return a.exec();
}
