#ifndef WIDGET_H
#define WIDGET_H

#include <QThread>
#include <QWidget>

class QPropertyAnimation;
class CustomGLWidget;

class ImgThread :public QThread
{
    Q_OBJECT
public:
    ImgThread(){}
signals:
    void sigMsg(const QImage &);
protected:
    void run(void);
};

class widget : public QWidget
{
    Q_OBJECT

public:
    explicit widget(QWidget *parent = nullptr);
    ~widget();

public slots:
    void playAnimation();

private:
    QImage m_img;
    ImgThread m_t;
    QPropertyAnimation* m_rollingAni{ nullptr };
    CustomGLWidget* m_GLWidget{ nullptr };
};

#endif // WIDGET_H
