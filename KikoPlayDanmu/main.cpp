#include <QtWidgets/QApplication>
#include <QLabel>

#include "LibraryManagers/Plugin/PluginManager.h"
#include "widget.h"

#ifndef QSTRING_U
#define QSTRING_U
#define u(x) QStringLiteral(x)
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString dir = a.applicationDirPath();

	PluginManager* pm = PluginManager::GetPluginManager();
	pm->loadPlugin(dir.toStdString());//����exe·�� ����

    widget w;
    w.setFixedSize(520, 480);


    QLabel* label = new QLabel(&w);
    label->setGeometry(50, 50, 100, 30);
    label->setText(u("СС����,��Ц��Ц"));

    w.show();
    return a.exec();
}
