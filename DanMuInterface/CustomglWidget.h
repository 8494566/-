#ifndef CUSTOMGLWIDGET_H
#define CUSTOMGLWIDGET_H

#include <iostream>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>

class CustomGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:

	enum
	{
		Left_Bottom_X,
		Left_Bottom_Y,
		Right_Bottom_X,
		Right_Bottom_Y,
		Right_Top_X,
		Right_Top_Y,
		Left_Top_X,
		Left_Top_Y,
		Pos_Max
	};

	CustomGLWidget(QWidget* parent = nullptr);
	~CustomGLWidget();

	// 设置实时显示的数据源
	virtual void setImageData(uchar* imageSrc, uint width, uint height);
	virtual void setImageData(const QImage& img);

protected:
	// 重写QGLWidget类的接口
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);

private:
	uchar* imageData_;           //纹理显示的数据源
	QSize imageSize_;            //图片尺寸
	QSize Ortho2DSize_;          //窗口尺寸
	QOpenGLTexture* texture_;
	GLuint textureId_;           //纹理对象ID
	int vertexPos_[Pos_Max];     //窗口坐标
	float texturePos_[Pos_Max];  //纹理坐标
	QImage m_img;
};

#endif // CUSTOMGLWIDGET_HPP
