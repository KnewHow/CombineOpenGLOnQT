#include "../includes/widget.h"
#include <QSurfaceFormat>
#include <QtMath>
#include <QtDebug>


Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent),
      VBO(QOpenGLBuffer::VertexBuffer),
      texture1(QOpenGLTexture::Target2D),
      texture2(QOpenGLTexture::Target2D)
{

    vertices = {
        // 位置                  //纹理坐标
         0.5f, -0.5f, 0.0f,     1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,     0.0f, 1.0f,   // 左下
         0.5f,  0.5f, 0.0f,     1.0f, 1.0f,   // 右上
       };

       timer.setInterval(18);
       connect(&timer,&QTimer::timeout,this,static_cast<void (Widget::*)()>(&Widget::update));
       timer.start();
}

Widget::~Widget(){
    texture1.destroy();
    texture2.destroy();
}

void Widget::initializeGL() {
    this->initializeOpenGLFunctions();
    QString verterShaderPath = "/home/knewhow/dev/QTWorkspace/CombineOpenGLOnQT/Resources/shader/vertex.glsl";
    QString fragmentShaderPath = "/home/knewhow/dev/QTWorkspace/CombineOpenGLOnQT/Resources/shader/fragment.glsl";
    if(!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, verterShaderPath)) {
        qDebug() << "Compile vertex shader Error: " << shaderProgram.log();
    }

    if(!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderPath)) {
        qDebug() << "Compile fragment shader Error: " << shaderProgram.log();
    }

    if(!shaderProgram.link()) {
        qDebug() << "Link shader Error: " << shaderProgram.log();
    }

    QOpenGLVertexArrayObject::Binder{&VAO};

    VBO.create();
    VBO.bind();
    VBO.allocate(vertices.data(), sizeof(float) * vertices.size());

    shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 5);
    shaderProgram.enableAttributeArray(0);
    shaderProgram.setAttributeBuffer(1, GL_FLOAT, sizeof(float) * 3, 2, sizeof(float) * 5);
    shaderProgram.enableAttributeArray(1);

    texture1.create();
    texture1.setData(QImage("/home/knewhow/dev/QTWorkspace/CombineOpenGLOnQT/Resources/texture/openglLogo.jpg").mirrored());
    texture1.setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
    texture1.setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    texture1.setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);

    texture2.create();
    texture2.setData(QImage("/home/knewhow/dev/QTWorkspace/CombineOpenGLOnQT/Resources/texture/sea.jpg").mirrored());
    texture2.setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
    texture2.setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    texture2.setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
}

void Widget::resizeGL(int w, int h) {
    this->glViewport(0, 0, w, h);
}

void Widget::paintGL() {
    this->glClearColor(0.1f, 0.5f, 0.7f, 1.0f);
    this->glClear(GL_COLOR_BUFFER_BIT);
    shaderProgram.bind();
    {
        float millSeconds = QTime::currentTime().msecsSinceStartOfDay() / 1000.0f;
        QMatrix4x4 trans;
        trans.translate(0.0f, 0.5 * qAbs(qSin(millSeconds)), 0.0);
        trans.scale(0.5 * qAbs(qSin(millSeconds)), 0.5 * qAbs(qSin(millSeconds)));
        trans.rotate(360.0f * millSeconds, 0.0f, 0.0f, -1.0f);
        shaderProgram.setUniformValue("trans", trans);
        QOpenGLVertexArrayObject::Binder{&VAO};
        texture1.bind(0);
        shaderProgram.setUniformValue("texture_color1", 0);
        texture2.bind(1);
        shaderProgram.setUniformValue("texture_color2", 1);
        this->glDrawArrays(GL_POLYGON, 0, 4);
        qInfo() << "millSeconds: " << millSeconds;
    }

}

