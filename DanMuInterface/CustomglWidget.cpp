#include "customglwidget.h"

#include <QDebug>
#include <QPainter>


const char* vertexSource =
        "#version 330\n"
        "layout(location = 0) in vec2 position;\n"
        "layout(location = 1) in vec3 incolor;\n"
        "out vec4 color;\n"
        "void main( void )\n"
        "{\n"
        " gl_Position = vec4(position, 0.0, 1.0);\n"
        " color = vec4(incolor, 1.0);\n"
        "}\n";

const char* fragmentSource =
        "#version 330\n"
        "in vec4 color;\n"
        "out vec4 outColor;\n"
        "void main( void )\n"
        "{\n"
        " outColor = color;\n"
        "}\n";

CustomGLWidget::CustomGLWidget(QWidget *parent) : QOpenGLWidget(parent) 
{
    // store triangle vertex coordinate & color data
	m_imageData = nullptr;
	setAutoFillBackground(false);
}

CustomGLWidget::~CustomGLWidget()
{
	m_texture->destroy();
}

void CustomGLWidget::setImageData(uchar* imageSrc, uint width, uint height)
{
	m_imageData = imageSrc;
	m_imageSize.setWidth(width);
	m_imageSize.setHeight(height);
	m_vertexPos[Left_Bottom_Y] = height;
	m_vertexPos[Right_Bottom_X] = width;
	m_vertexPos[Right_Bottom_Y] = height;
	m_vertexPos[Right_Top_X] = width;
	update();
}

void CustomGLWidget::setImageData(const QImage& img)
{
	m_img = img;
	m_imageData = (uchar*)m_img.bits();
	m_imageSize.setWidth(m_img.width());
	m_imageSize.setHeight(m_img.height());
	m_vertexPos[Left_Bottom_Y] = m_img.height();
	m_vertexPos[Right_Bottom_X] = m_img.width();
	m_vertexPos[Right_Bottom_Y] = m_img.height();
	m_vertexPos[Right_Top_X] = m_img.width();
	update();
}

void CustomGLWidget::setImageDataPost(int x, int y)
{
	if (m_imageData == nullptr) {
		return;
	}
	m_vertexPos[Left_Top_X] = x;
	m_vertexPos[Left_Top_Y] = y;
	m_vertexPos[Left_Bottom_X] = x;
	m_vertexPos[Left_Bottom_Y] = m_imageSize.height() + y;
	m_vertexPos[Right_Bottom_X] = m_imageSize.width() + x;
	m_vertexPos[Right_Bottom_Y] = m_imageSize.height() + y;
	m_vertexPos[Right_Top_X] = m_imageSize.width() + x;
	m_vertexPos[Right_Top_Y] = y;
	update();
}

QPoint CustomGLWidget::getCustomGLWidgetPos()
{
	return QPoint(m_vertexPos[Left_Top_X], m_vertexPos[Left_Top_Y]);
}

void CustomGLWidget::setCustomGLWidgetPos(QPoint pos)
{
	setImageDataPost(pos.x(), pos.y());
}


void CustomGLWidget::initializeGL() {
	initializeOpenGLFunctions();
	m_texture = new QOpenGLTexture(QOpenGLTexture::Target2D);

	m_texture->create();
	m_textureId = m_texture->textureId();
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void CustomGLWidget::resizeGL(int w, int h) 
{
    // only drawing the 2d surface so no need to modify any projections etc;
	m_Ortho2DSize.setWidth(w);
	m_Ortho2DSize.setHeight(h);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, m_Ortho2DSize.width(), m_Ortho2DSize.height(), 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}

void CustomGLWidget::paintGL() {
	static bool initTextureFlag = false;
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_imageData == nullptr) {
		return;
	}

	//QByteArray ba((const char*)imageData_,64);
	//qDebug() <<imageData_ << ba;
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	if (!initTextureFlag) {
		// 首次显示纹理
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_imageSize.width(), m_imageSize.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_imageData);
		initTextureFlag = true;
	}
	else {
		// 动态修改纹理数据
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_imageSize.width(), m_imageSize.height(), GL_RGBA, GL_UNSIGNED_BYTE, m_imageData);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageSize_.width(), imageSize_.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData_);
	}

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);

	//顶点坐标和纹理坐标必须一一对应
	glTexCoord2d(m_texturePos[Left_Top_X], m_texturePos[Left_Top_Y]);
	glVertex2d(m_vertexPos[Left_Top_X], m_vertexPos[Left_Top_Y]);
	glTexCoord2d(m_texturePos[Left_Bottom_X], m_texturePos[Left_Bottom_Y]);
	glVertex2d(m_vertexPos[Left_Bottom_X], m_vertexPos[Left_Bottom_Y]);
	glTexCoord2d(m_texturePos[Right_Bottom_X], m_texturePos[Right_Bottom_Y]);
	glVertex2d(m_vertexPos[Right_Bottom_X], m_vertexPos[Right_Bottom_Y]);
	glTexCoord2d(m_texturePos[Right_Top_X], m_texturePos[Right_Top_Y]);
	glVertex2d(m_vertexPos[Right_Top_X], m_vertexPos[Right_Top_Y]);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}