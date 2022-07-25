#include "widget.h"

#include <QtWidgets/QApplication>
#include <QPainter>
#include <QTimer>
#include <QPropertyAnimation>

#include "LibraryManagers/Plugin/PluginManager.h"
#include "DanMuInterface/Interface/DanMuInterface.h"
#include "DanMuInterface/CustomglWidget.h"


widget::widget(QWidget *parent) :
    QWidget(parent)
{
	PluginManager* pm = PluginManager::GetPluginManager();

	DanMuInterface* Interface = (DanMuInterface*)pm->getPlugin("DanMuInterface");
    m_GLWidget = Interface->createCustomGLWidget(this);

    m_rollingAni = new QPropertyAnimation();

    connect(&m_t,&ImgThread::sigMsg,this,[=](const QImage &img){m_GLWidget->setImageData(img);});

    m_t.setProperty("PARENT",QVariant::fromValue(qobject_cast<QObject*>(this)));
    m_t.start();

    QTimer* rollingTimer = new QTimer(this);
    rollingTimer->setInterval(3000);
    connect(rollingTimer, SIGNAL(timeout()), this, SLOT(playAnimation()));
    rollingTimer->start();
}

widget::~widget()
{
    if(m_t.isRunning()){
        m_t.requestInterruption();
        m_t.quit();
        m_t.wait();
    }
}

void widget::playAnimation()
{
    m_rollingAni->setTargetObject(m_GLWidget);
    m_rollingAni->setPropertyName("pos");
    m_rollingAni->setStartValue(QPoint(0, 0));
    m_rollingAni->setEndValue(QPoint(width(), 0));
    m_rollingAni->setDuration(2000);
    m_rollingAni->start();
}

// 用线程来发消息给widget进行显示
void ImgThread::run()
{
    QVariant var;
    QObject* obj;
    QImage image;
    static uint i = 0,id,allLoaded = 0;
    QString dir = QCoreApplication::applicationDirPath();

    var = this->property("PARENT");
    obj = var.value<QObject*>();
    widget *p = qobject_cast<widget *>(obj);
    if (!isInterruptionRequested())
    {
        image.load(dir + QString("/images/lena2.jpg"));
        emit sigMsg(image.rgbSwapped());
    }
}
